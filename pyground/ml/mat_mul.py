import tensorflow as tf
import numpy as np

n, m = 20, 30
sa, sb = (n, m), (m, n)
ma = np.random.rand(n, m)
mb = np.random.rand(m, n)

# Creates a graph.
with tf.device('/gpu:0'):
    a = tf.constant(ma, shape=sa, name='a')
    b = tf.constant(mb, shape=sb, name='b')
c = tf.matmul(a, b)
# Creates a session with log_device_placement set to True.
sess = tf.Session(config=tf.ConfigProto(log_device_placement=True))
# Runs the op.
print(sess.run(c))
