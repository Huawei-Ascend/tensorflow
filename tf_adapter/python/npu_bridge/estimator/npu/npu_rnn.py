import tensorflow as tf

def npu_dynamic_rnn(cell,
            inputs,
            initial_state=None,
            dtype=None,
            sequence_length=None,
            scope=None):
  """Creates a high performance neural network specified by RNNCell `cell`.
  Args:
    cell: An instance of RNNCell.
    inputs: A length T list of inputs, each a `Tensor` of shape `[max_time,
      batch_size, input_size]`, or a nested tuple of such elements.
    initial_state: (optional) An initial state for the RNN. If `cell.state_size`
      is an integer, this must be a `Tensor` of appropriate type and shape
      `[batch_size, cell.state_size]`. If `cell.state_size` is a tuple, this
      should be a tuple of tensors having shapes `[batch_size, s] for s in
      cell.state_size`.
    dtype: (optional) The data type for the initial state and expected output.
      Required if initial_state is not provided or RNN state has a heterogeneous
      dtype.
    sequence_length: Specifies the length of each sequence in inputs. An int32
      or int64 vector (tensor) size `[batch_size]`, values in `[0, T)`.
    scope: VariableScope for the created subgraph; defaults to "rnn".
  """
  # tf origin static_rnn
  inputs = tf.unstack(inputs, axis=0)
  encoder_outputs , encoder_state = tf.nn.static_rnn(
    cell,
    inputs,
    initial_state=initial_state,
    dtype=dtype,
    sequence_length=sequence_length,
    scope=scope)
  encoder_outputs = tf.stack(encoder_outputs, axis=0)

  return encoder_outputs, encoder_state