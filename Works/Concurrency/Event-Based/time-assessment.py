''' Question 6.
how can you tell if the effort to build an asynchronous, event-based approach are worth it? 
Can you create an experiment to show the benefits? 
How much implementation complexity did your approach add?

Subjects : file server with synchronous I/O (tcp-file-server.c) - port.12345 vs. file server with asynchronous I/O (tcp-file-async.c) - port.12346
Criteria : time, number of codes
Hypothesis : I already know that number of codes is much larger in asynchronous I/O.
    then, at least time consumed should be less.
    Since asynchronous I/O solve the problem when synchronous I/O enter to blocked situation, 
    it can works better in asynchronous case, leveraging concurrency.

Script : time-assessment.py
Read File : californiaDreamin.txt 

Result : async I/O is not faster than sync I/O. it's even slower. WHY????
    sync : Total time for 10000 requests: 5.07 seconds
    async : Total time for 10000 requests: 5.10 seconds

my opinion : 
    maybe there can be a chance that async way consumes more time than sync way. 
    
    from the beginning, why do we use async I/O? 
    because it can help run other I/O requests, even the requested I/O is blocked or something. 
    Then, if there isn't any blocked request, we might can't assure that async way is faster than sync way. 

    And in our code, we just read a single txt file, which rarely leads to page fault or other blocked situation. 

    Maybe that can be the reason why async way is not faster than sync I/O.

    Also, async I/O has more code lines and more complicated.
    therefore it makes sence that it's even slower than sync I/O.

'''
# client_sim.py
import socket
import threading
import time

HOST = 'localhost'
PORT = 12346
NUM_CLIENTS = 200      # number of concurrent clients
REQUESTS_PER_CLIENT = 50  # number of requests each client sends
FILENAME = 'californiaDreamin.txt'    # file to request

def client_task(client_id):
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((HOST, PORT))
        for i in range(REQUESTS_PER_CLIENT):
            msg = FILENAME + '\n'
            s.sendall(msg.encode())
            data = s.recv(4096)
            print(f'Client {client_id}, request {i+1}: {len(data)} bytes received')
            time.sleep(0.1)  # small delay between requests
        s.close()
    except Exception as e:
        print(f'Client {client_id} error: {e}')

threads = []
start_time = time.time()
for i in range(NUM_CLIENTS):
    t = threading.Thread(target=client_task, args=(i,))
    t.start()
    threads.append(t)

for t in threads:
    t.join()

end_time = time.time()
print(f'Total time for {NUM_CLIENTS*REQUESTS_PER_CLIENT} requests: {end_time - start_time:.2f} seconds')


