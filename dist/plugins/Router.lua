function test()
    print("OK")
end

function read_file(path)
    -- Opens a file in read
    local file = io.open(path, "r")

    -- sets the default input file
    io.input(file)

    -- prints the first line of the file
    local content = io.read("*all")

    -- closes the open file
    io.close(file)

    return content
end

function handle()
    local url = request:get("REQUEST_URI")

    if (url == "/404") then
        response.status = 200
        response.type = "text/html"
        print(read_file("plugins/Router/404.html"))
        return
    end

    response.status = 307;
    response:set("Location", "/404");
end