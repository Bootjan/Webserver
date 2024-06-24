import requests

# url = "http://127.0.0.1:8080"
# x = requests.get(url)
# print(x.text)

url = "http://127.0.0.1:8080/test_dir.txt"
x = requests.get(url)
print(x.text)

url = "http://127.0.0.1:4242"
x = requests.get(url)
print(x.text)

# url = "http://127.0.0.1:4242"
# x = requests.delete(url)
# print(x.text)

# url = "http://127.0.0.1:8080/subscibe.test"
# x = requests.delete(url)
# print(x.text)
