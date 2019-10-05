local fileReader = require "scripts.lua.FileReader"

routes = {
    PageNotFound = "/404"
}

local router = {
    [routes.PageNotFound] = function ()
        response.status = 404
        print(fileReader.readAll("plugins/Router/404.html"))
    end
}

function test()

end

function handle()
    local url = request:get("REQUEST_URI")

    setResponseDefaults()

    if router[url] ~= nil then
        router[url]()
        return
    end

    redirectTo(routes.PageNotFound);
end

function setResponseDefaults()
    response.type = "text/html"
end

function redirectTo(url)
    response.status = 307;
    response:set("Location", url);
end