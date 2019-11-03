local fileReader = require "scripts.lua.FileReader"

routes = {
    PageNotFound = "/404",
    SampleRoute = "/sample-route"
}

local router = {
    [routes.PageNotFound] = function(queryString)
        response.status = 404
        response.content = fileReader.readAll("plugins/Router/404.html")
    end,
    [routes.SampleRoute] = function(queryString)
        response.status = 200
        response.content = "This is a sample route from Lua Router. :)"
    end
}

function test()
end

function handle()
    local url = request:getUrl()
    local queryString = request:getQueryString()

    response:setHeader("Content-Type", "text/html; charset=UTF-8")

    if router[url] == nil then
        redirectTo(routes.PageNotFound .. "?uri=" .. url)
        return
    end

    router[url](queryString)
end

function redirectTo(url)
    response.status = 302
    response:setHeader("Location", url)
end