import tensorflow as tf
from six.moves import queue as Queue
import threading
from tensorflow.python.training import session_run_hook
from tensorflow.python.training import basic_session_run_hooks
from tensorflow.python.platform import tf_logging as logging
from npu_bridge.estimator import npu_ops
from npu_bridge.hccl import hccl_ops
from tensorflow.python.ops import summary_ops_v2 as contrib_summary
from tensorflow.core.protobuf import config_pb2
import time
from npu_bridge.estimator.npu import util

# Constant
_BATCH_SIZE_KEY = 'batch_size'
_RESERVED_PARAMS_KEYS = [_BATCH_SIZE_KEY]

_NPU_RUNCONFIG = 'npu_runconfig'
_ITERATIONS_PER_LOOP_VAR = 'iterations_per_loop'
_LOOP_COND_VAR = 'loop_cond'
_CONST_ZERO = 'zero'
_CONST_ONE = 'one'

util.register_func(_ITERATIONS_PER_LOOP_VAR)
util.register_func(_LOOP_COND_VAR)
util.register_func(_CONST_ZERO)
util.register_func(_CONST_ONE)

class NPUShutDownHook(session_run_hook.SessionRunHook):
    """Hook to shutdown the system."""

    def __init__(self, scaffold=None):
        super(NPUShutDownHook, self).__init__()
        self._scaffold = scaffold
        self._shutdown_npu_op = None

    def begin(self):
        if not self._shutdown_npu_op or self._shutdown_npu_op.graph != tf.get_default_graph():
            self._shutdown_npu_op = npu_ops.NPUShutdown()

    def end(self, session):
        logging.info("NPUShutDownHook run...")
        session.run(self._shutdown_npu_op)


class NPUBroadcastGlobalVariablesHook(session_run_hook.SessionRunHook):
    """Broadcasts initial variable states from rank 0 to all other processes.

    This is necessary to ensure consistent initialization of all workers when
    training is started with random weights or restored from a checkpoint.

    """
    def __init__(self, root_rank=None, index=None):
        """Construct a new NPUBroadcastGlobalVariablesHook that will broadcast all
        global variables from root rank to all other processes during initialization.

        Args:
          root_rank:
            Rank that will send data, other ranks will receive data.
          index:
            Current rand id.
        """
        self._root_rank = root_rank
        self._index = index
        self._bcast_op = None

    def begin(self):
        if not self._bcast_op or self._bcast_op.graph != tf.get_default_graph():
            self._bcast_op = broadcast_global_variables(self._root_rank, self._index)

    def after_create_session(self, session, coord):
        logging.info("NPUBroadcastGlobalVariablesHook run...")
        session.run(self._bcast_op)


class NPUCheckpointSaverHook(basic_session_run_hooks.CheckpointSaverHook):
    """Saves checkpoints every N steps or seconds."""
    def __init__(self,
                 checkpoint_dir,
                 save_secs=None,
                 save_steps=None,
                 saver=None,
                 checkpoint_basename="model.ckpt",
                 scaffold=None,
                 listeners=None):
        """Initializes a `CheckpointSaverHook`.

        Args:
            checkpoint_dir: `str`, base directory for the checkpoint files.
            save_secs: `int`, save every N secs.
            save_steps: `int`, save every N steps.
            saver: `Saver` object, used for saving.
            checkpoint_basename: `str`, base name for the checkpoint files.
            scaffold: `Scaffold`, use to get saver object.
            listeners: List of `CheckpointSaverListener` subclass instances.
                Used for callbacks that run immediately before or after this hook saves
                the checkpoint.

        Raises:
            ValueError: One of `save_steps` or `save_secs` should be set.
            ValueError: At most one of saver or scaffold should be set.
        """
        super(NPUCheckpointSaverHook, self).__init__(
            checkpoint_dir=checkpoint_dir,
            save_secs=save_secs,
            save_steps=save_steps,
            saver=saver,
            checkpoint_basename=checkpoint_basename,
            scaffold=scaffold,
            listeners=listeners)

    def after_run(self, run_context, run_values):
        global_step = run_context.session.run(self._global_step_tensor)
        logging.info("global_step..." + str(global_step))
        super().after_run(run_context, run_values)

    def end(self, session):
        logging.info("NPUCheckpointSaverHook end...")
        super().end(session)

class SetIterationsVarHook(session_run_hook.SessionRunHook):
    def __init__(self, iterations_per_loop=None):
        self._iterations_per_loop = iterations_per_loop

    def begin(self):
        self._iterations_per_loop_var = util.create_or_get_var(_ITERATIONS_PER_LOOP_VAR)
        self._loop_cond_var = util.create_or_get_var(_LOOP_COND_VAR)
        self._const_zero = util.create_or_get_var(_CONST_ZERO)
        self._const_one = util.create_or_get_var(_CONST_ONE)

    def after_create_session(self, session, coord):
        self._iterations_per_loop_var.load(self._iterations_per_loop - 1, session=session)
        self._loop_cond_var.load(0, session=session)
        self._const_zero.load(0, session=session)
        self._const_one.load(1, session=session)
        print("load iterations_per_loop value -----------")
        print(session.run(self._iterations_per_loop_var))

def broadcast_global_variables(root_rank, index):
    """Broadcasts all global variables from root rank to all other processes.
    Arguments:
        root_rank: rank of the process from which global variables will be broadcasted
        to all other processes.
    """
    op_list = []
    for var in tf.global_variables():
        # the input and out tensor of HCOMBroadcast interface are list
        inputs = [var]
        outputs=hccl_ops.broadcast(tensor=inputs,root_rank=root_rank)
        if outputs is not None:
            op_list.append(outputs[0].op)
            op_list.append(tf.assign(var, outputs[0]))

    return tf.group(op_list)

class _SIGNAL(object):
  STOP = -1

class _OpQueueContext(object):
  """Manages work queue and thread for a infeed/outfeed thread."""

  def __init__(self, name, target, args):
    self._name = name
    self._queue = Queue.Queue()
    args = (self,) + args
    self._thread = threading.Thread(name=name, target=target, args=args)
    self._thread.daemon = True
    self._thread.start()

  def stop(self):
    self._queue.put(_SIGNAL.STOP)

  def join(self):
    logging.info('Shutting down %s thread.' % self._name)
    self.stop()
    self._thread.join()

class NPULogOutfeedSessionHook(session_run_hook.SessionRunHook):
  def __init__(self, output_stream):
    self._output_stream = output_stream
    self._stopped = False

  def begin(self):
    self._finalize_ops = [npu_ops.stop_outfeed_dequeue_op("_npu_log")]
    outfeed_log_tensors = npu_ops.outfeed_dequeue_op(
            channel_name="_npu_log",
            output_types=[tf.string],
            output_shapes=[()])
    self._dequeue_ops = tf.print(outfeed_log_tensors, output_stream=self._output_stream)

  def _run_coordinate(self, queue_ctx, session, coord):
    logging.info('Starting log outfeed thread coordinate.')
    while not coord.should_stop():
      time.sleep(1)
    if not self._stopped:
      self._stopped = True
      session.run(self._finalize_ops)

  def _run_outfeed(self, queue_ctx, session):
    logging.info('Starting log outfeed thread controller.')
    while True:
        try:
          session.run(self._dequeue_ops)
        except tf.errors.OutOfRangeError:
          logging.info('Log outfeed thread finished')
          break
        except Exception as e:
          logging.error('Log outfeed thread exit unexpectedly.', e.what())
          break

  def after_create_session(self, session, coord):
    self._outfeed_controller = _OpQueueContext(
        name='LogOutfeedController', target=self._run_outfeed, args=(session,))
    self._outfeed_coordinate = _OpQueueContext(
        name='LogOutfeedCoordinate', target=self._run_coordinate, args=(session, coord))
    logging.info('Add log output coordinate thread to coord')
    #add outfeed_coordinate thread to coord
    #when the coordinated session close, the coord.join() will wait for this thread finish
    coord.register_thread(self._outfeed_coordinate._thread)

  def end(self, session):
    if not self._stopped:
      self._stopped = True
      session.run(self._finalize_ops)
    logging.info('Stop log output thread controller')
    self._outfeed_controller.join()

class NPUInfeedOutfeedSessionHook(session_run_hook.SessionRunHook):
  def __init__(self,
               dequeue_ops,
               channel_name):
    self._dequeue_ops = dequeue_ops
    self._channel_name = channel_name
    self._finished = False
    self._stopped = False

  def begin(self):
    self._init_ops = []
    self._finalize_ops = [npu_ops.stop_outfeed_dequeue_op(self._channel_name)]

    summary_writer_init_ops = contrib_summary.summary_writer_initializer_op()
    self._init_ops.extend(summary_writer_init_ops)
    # Get all the writer resources from the initializer, so we know what to flush.
    for op in summary_writer_init_ops:
      self._finalize_ops.append(contrib_summary.flush(writer=op.inputs[0]))

  def _run_coordinate(self, queue_ctx, session, coord):
    logging.info('Starting outfeed thread coordinate.')
    while not coord.should_stop():
      time.sleep(1)
    if not self._stopped:
      self._stopped = True
      session.run(self._finalize_ops)

  def _run_outfeed(self, queue_ctx, session):
    logging.info('Starting outfeed thread controller.')
    while True:
        try:
          session.run(self._dequeue_ops)
        except tf.errors.OutOfRangeError:
          logging.info('summary outfeed thread finished')
          break
        except Exception as e:
          logging.error('summary outfeed thread exit unexpectedly.', e.what())
          break
    logging.info('Outfeed thread finished, shutting down.')

  def after_create_session(self, session, coord):
    logging.info('Init NPU system')
    start = time.time()
    session.run(self._init_ops,
                options=config_pb2.RunOptions(timeout_in_ms=5 * 60 * 1000))
    logging.debug('Initialized NPU in %d seconds', time.time() - start)

    self._outfeed_controller = _OpQueueContext(
        name='OutfeedController', target=self._run_outfeed, args=(session,))
    self._outfeed_coordinate = _OpQueueContext(
        name='OutfeedCoordinate', target=self._run_coordinate, args=(session, coord))
    logging.info('Add log output coordinate thread to coord')
    #add outfeed_coordinate thread to coord
    #when the coordinated session close, the coord.join() will wait for this thread finish
    coord.register_thread(self._outfeed_coordinate._thread)

  def end(self, session):
    self._finished = True

    logging.info('Shutdown NPU system.')
    if not self._stopped:
      self._stopped = True
      session.run(self._finalize_ops)

    logging.info('Stop output thread controller')
    self._outfeed_controller.join()

class NPUOutputTensorHook(basic_session_run_hooks.LoggingTensorHook):
    """call output_fn to print tensors every N steps or at end."""

    def __init__(self, tensors,
                 dependencies=None,
                 output_fn=None,
                 output_every_n_steps=0
                 ):
        """Initializes a `NPUOutputTensorHook`.

        Args:
            tensors: `dict` that maps string-valued tags to tensors/tensor names,
                or `iterable` of tensors/tensor names.
            dependencies: control edges.
            output_fn: A callable, uses __call__ to print tensors
            output_every_n_steps: `int`, print the values of `tensors` once every N local
            steps taken on the current worker.

        """
        self._tensors = None
        self._output_fn = output_fn
        self._output_every_n_steps = output_every_n_steps
        self._output_list = []
        self._iter_count = 0
        if tensors is not None:
            if dependencies is not None:
                if not isinstance(dependencies, (tuple, list)):
                    dependencies = [dependencies]

                with tf.control_dependencies(dependencies):
                    self._tensors = {k : tf.identity(v) for k, v in tensors.items()}
            else:
                self._tensors = tensors

            super(NPUOutputTensorHook, self).__init__(self._tensors, every_n_iter=1 << 31)

    def begin(self):
        logging.info("NPUOutputTensorHook begin...")
        if self._tensors is not None:
            super(NPUOutputTensorHook, self).begin()

    def before_run(self, run_context):
        logging.info("NPUOutputTensorHook before_run...", self._tensors)
        if self._tensors is not None:
            return tf.train.SessionRunArgs(self._current_tensors)

    def after_run(self, run_context, run_values):
        logging.info("NPUOutputTensorHook after_run...", run_values.results)
        _ = run_context
        if self._tensors is not None:
            self._stash_outputs(run_values.results)

        self._iter_count += 1
        if self._iter_count % self._output_every_n_steps == 0:
            self._call_output_fn()

    def end(self, session):
        logging.info("NPUOutputTensorHook end...")
        if self._output_list is not None and len(self._output_list):
            self._call_output_fn()

    def _stash_outputs(self, tensor_values):
        self._output_list.append({tag : tensor_values[tag] for tag in self._tag_order})

    def _call_output_fn(self):
        self._output_fn.__call__(self._output_list)
        del self._output_list[:]
