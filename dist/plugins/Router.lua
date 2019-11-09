local fileReader = require "scripts.lua.io.FileReader"
local sidecar = require "scripts.lua.controllers.Sidecar"

routes = {
    PageNotFound = "/404",
    SampleRoute = "/sample-route",
    Sidecar = "/sidecar",
}

local router = {
    [routes.PageNotFound] = function(request, response)
        response.status = 404
        response.content = fileReader:readAll("plugins/Router/404.html"):gsub("%{$route}", request:getQueryString())
    end,
    [routes.SampleRoute] = function(request, response)
        response.status = 200
        response.content = "This is a sample route from Lua Router. :)"
    end,
    [routes.Sidecar] = function(request, response)
        sidecar:handle(request, response)
    end
}

function test()
end

function handle()
    local url = request:getUrl()
    local handler = router[url]

    response:setHeader("Content-Type", "text/html; charset=UTF-8")

    if handler == nil then
        redirectTo(routes.PageNotFound .. "?uri=" .. url)
        return
    end

    handler(request, response)
end

function redirectTo(url)
    response.status = 302
    response:setHeader("Location", url)
end