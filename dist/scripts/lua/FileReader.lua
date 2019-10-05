require "io"

local FileReader = {}

function FileReader.readAll(path)
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

return FileReader