print("<h1>Exemplo de página com LUA</h1>")
print("<p><b>Scripts localizados em wroot/lua.</b></p>")

for i = 0, 24 do
    print("Testando latência limite... ")

    if i % 6 == 0 then
        print("<br/>")
    end
end

print("<br/>")
print("<p>Script executado com sucesso!</p>")