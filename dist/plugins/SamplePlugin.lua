function test()
    if true ~= true then
        error('Test failed.')
    end

    print("OK")
end

function handle()
    print("I do nothing")
end