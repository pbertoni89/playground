"""
This test gives an example of how some computation results from an
auxiliary thread could be 'published' via pubsub in a thread-safe
manner, in a 'gui'-like application, ie an application where the
main thread is in an infinite event loop and supports the callback
of user-defined functions when the gui is idle.
The worker thread 'work' is to increment a counter
as fast as interpreter can handle. Every so often (every resultStep counts),
the thread stores the count in a synchronized queue, for later retrieval
by the main thread. In parallel to this, the main thread loops forever (or
until user interrupts via keyboard), doing some hypothetical work
(represented by the sleep(1) call) and calling all registered 'idle'
callbacks. The transfer is done by extracting items from the queue and
publishing them via pubsub.
Oliver Schoenborn
May 2009
:copyright: Copyright 2008-2009 by Oliver Schoenborn, all rights reserved.
:license: BSD, see LICENSE.txt for details.
"""

from queue import Queue
import time
import threading
import sys

from pubsub import pub

__author__ = "schoenb"
__date__ = "$31-May-2009 9:11:41 PM$"

resultStep = 1000000  # how many counts for thread "result" to be available


def thread_observer(transfers, thread_obj, count):
    """Listener that listens for data from testTopic. This function
    doesn't know where the data comes from (or in what thread it was
    generated... but threadObj is the thread in which this
    threadObserver is called and should indicate Main thread)."""

    print(transfers, thread_obj, count / resultStep)


pub.subscribe(thread_observer, 'testTopic')


def on_idle():
    """This should be registered with 'gui' to be called when gui is idle
    so we get a chance to transfer data from aux thread without blocking
    the gui. Ie this function must spend as little time as possible so
    'gui' remains reponsive."""
    thread.transfer_data()


class ParaFunction(threading.Thread):
    """
    Represent a function running in a parallel thread. The thread
    just increments a counter and puts the counter value on a synchronized
    queue every resultStep counts. The content of the queue can be published by
    calling transferData().
    """

    def __init__(self):
        threading.Thread.__init__(self)
        self.running = False  # set to True when thread should stop
        self.count = 0  # our workload: keep counting!
        self.queue = Queue()  # to transfer data to main thread
        self.transfer = 0  # count how many transfers occurred

    def run(self):
        print('aux thread started')
        self.running = True
        while self.running:
            self.count += 1
            if self.count % resultStep == 0:
                self.queue.put(self.count)

        print('aux thread done')

    def stop(self):
        self.running = False

    def transfer_data(self):
        """Send data from aux thread to main thread. The data was put in
        self.queue by the aux thread, and this queue is a Queue.Queue which
        is a synchronized queue for inter-thread communication.
        Note: This method must be called from main thread."""
        self.transfer += 1
        while not self.queue.empty():
            pub.sendMessage('testTopic',
                            transfers=self.transfer,
                            threadObj=threading.currentThread(),
                            count=self.queue.get())


thread = ParaFunction()


def main():
    l_idle_fns = [on_idle]  # list of functions to call when 'gui' idle

    try:
        thread.start()

        print('starting event loop')
        do_loop = True
        while do_loop:
            time.sleep(1)  # pretend that main thread does other stuff
            for idleFn in l_idle_fns:
                idleFn()

    except KeyboardInterrupt:
        print('Main interrupted, stopping aux thread')
        thread.stop()

    except Exception as exc:
        print(exc)
        exc = sys.exc_info()[1]
        print(exc)
        print('Exception, stopping aux thread')
        thread.stop()


if __name__ == '__main__':
    main()
