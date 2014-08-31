function getAjaxData(script){
    var doc = $.parseHTML(script);
    $(doc).find("a").remove();
    $("body").html(doc);
}

function networkError(){
    $("body").html("System Error: network problem!");
}
