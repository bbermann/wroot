function test()
    print("OK")
end

function handle()
    print("<html><head><title>Router plugin for wRoot</title></head>")
    print("<body>")

    local url = request:get("REQUEST_URI")
    print(url)

    print("</body>")
    print("</html>")
end