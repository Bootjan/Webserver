import requests
import threading
import time
import sys

START_THREADS = 1;
MAX_THREADS = 32;

methods = [requests.get]#, requests.post, requests.delete]

url = sys.argv[1];

connection_attempts = 0;
connection_errors = 0;

running = True;
def thread_entry(method):
	global running, connection_attempts, connection_errors
	while (running):
		try:
			connection_attempts += 1;
			response = method(url)
		except requests.exceptions.ConnectionError:
			print("Connection error");
			connection_errors += 1;
		else:
			if (response.status_code != 200):
				print("Status: " + str(response.status_code))

threads = []
def start_thread():
	global methods

	methods = methods[1:] + [methods[0]]
	threads.append(threading.Thread(target=thread_entry, args=(methods[0],)))
	threads[-1].start()

for i in range(START_THREADS - 1):
	start_thread()

while (running and len(threads) < MAX_THREADS):
	start_thread()
	print("THREAD COUNT: " + str(len(threads)))
	time.sleep(4 + (len(threads) / MAX_THREADS) * 4);

running = False;
for thread in threads:
	thread.join()

print("TOTAL CONNECTION ATTEMPTS: %i" % (connection_attempts));
print("TOTAL CONNECTION ERRORS: %i" % (connection_errors));
print("PRECENTAGE FAILED %f%%" % ((float(connection_errors) / connection_attempts) * 100));