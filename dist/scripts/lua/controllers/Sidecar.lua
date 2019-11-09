local mongo = require 'mongo'
local client = mongo.Client('mongodb://127.0.0.1:33017')

local Sidecar = {
    beforeProxy = {
        requestLog = function(request)
            local requestBson = mongo.BSON {
                _id = mongo.ObjectID(),
                method = request.method,
                uri = request.uri,
                httpVersionMajor = request.httpVersionMajor,
                httpVersionMinor = request.httpVersionMinor,
            }

            client:getCollection('sidecar', 'request_log'):insert(requestBson)
        end,
    },
    afterProxy = {
        responseLog = function(response)
            local responseBson = mongo.BSON {
                _id = mongo.ObjectID(),
                status = response.status,
                content = response.content,
                httpVersionMajor = response.httpVersionMajor,
                httpVersionMinor = response.httpVersionMinor,
            }

            client:getCollection('sidecar', 'response_log'):insert(responseBson)
        end,
    }
}

function Sidecar:handle(request, response)
    local redirectUri = self:getRedirectUri(request)

    if redirectUri == nil then
        self:badRequest(response)
        return
    end

    self:runBeforeProxy(request)
    self:redirect(response, redirectUri)
    self:runAfterProxy(response)
end

function Sidecar:badRequest(response)
    response.status = 400
    response.content = "<h1>Bad Request</h1>" ..
            "<h2>The redirect argument is mandatory.<h2>"
end

function Sidecar:getRedirectUri(request)
    return request:getQueryString():match("redirect=([^&]+)")
end

function Sidecar:runBeforeProxy(request)
    for _, handler in next, self.beforeProxy, nil do
        handler(request)
    end
end

function Sidecar:runAfterProxy(response)
    for _, handler in next, self.afterProxy, nil do
        handler(response)
    end
end

function Sidecar:redirect(response, uri)
    response.status = 302
    response:setHeader("Location", uri)
end

return Sidecar