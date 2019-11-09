local requestCounter = 0

local Sidecar = {
    requestCounter = 0,
    middlewares = {
        requestCount = function(_, _)
            requestCounter = requestCounter + 1
        end
    }
}

function Sidecar:handle(request, response)
    local redirectUri = self:getRedirectUri(request)

    if redirectUri == nil then
        self:badRequest(response)
        return
    end

    self:runMiddlewares(request, response)
    self:redirect(response, redirectUri)
end

function Sidecar:badRequest(response)
    response.status = 400
    response.content = "<h1>Bad Request</h1>" ..
            "<h2>The redirect argument is mandatory.<h2>"
end

function Sidecar:getRedirectUri(request)
    return request:getQueryString():match("redirect=([^&]+)")
end

function Sidecar:runMiddlewares(request, response)
    for _, handler in next, self.middlewares, nil do
        handler(request, response)
    end
end

function Sidecar:redirect(response, uri)
    response.status = 302
    response:setHeader("Location", uri)
end

return Sidecar