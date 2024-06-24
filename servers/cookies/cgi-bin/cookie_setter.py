import sys

input_str = sys.stdin.read()

if "cookie_name=" not in input_str:
	exit

cookies = input_str.split('=')
cookie = cookies[1]

print('Set-Cookie: cookie_' + cookie + '=idk; Path=/;')
print('Server: 127.0.0.1:6969')
print('Content-Type: text/html\n')
print('''
<script>
	function	getCookie()
	{
		var elements = document.cookie.split(/[\s=;]+/);
		console.log(elements);
	}
	getCookie();
</script>
<h1>The cookie setter cgi worked: ''' + cookie + '''</h1>\n
<a href=\"/index.html\">Back to homepage</a>\n
''')