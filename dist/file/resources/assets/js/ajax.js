$(document).ready(function ()
{
    $('.ajax').click(function ()
    {
        $.ajax({
            type: 'POST',
            data: 'body-request=true',
            url: document.location.pathname,
            success: function (retorno)
            {
                $("body").html(retorno);
            }
        });
    });

    $('#menu-cadastro-geral').click(function () {
        $.ajax({
            type: 'POST',
            async: true,
            data: 'onClick=menu-cadastro-geral',
            success: function (retorno)
            {
                $("#menu-secundario").html(retorno);
            }
        });
    });
});
