local mongo = require 'mongo'
local client = mongo.Client('mongodb://127.0.0.1:33017')

local Sidecar = {
    middlewares = {
        requestLog = function(request, _)
            local requestBson = mongo.BSON {
                _id = mongo.ObjectID(),
                method = request.method,
                uri = request.uri,
                httpVersionMajor = request.httpVersionMajor,
                httpVersionMinor = request.httpVersionMinor,
            }

            client:getCollection('sidecar', 'request_log'):insert(requestBson)
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