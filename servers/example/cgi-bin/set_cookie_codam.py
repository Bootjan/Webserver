print('Set-Cookie: sletje=sletje; Path=/;')
print('Server: localhost:8080')
print('Content-Type: text/html\n\n')
print('''
<script>
	function	getCookie()
	{
		var elements = document.cookie.split('=');
		console.log(elements);
	}
	getCookie();
</script>
<h1>The codam cookie cgi with python worked</h1>
''')