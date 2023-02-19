var express = require('express');
var app = express();
var port = 3000;

//setting middleware
app.use(express.static('public'));

var server = app.listen(port);

console.log("Server Listening at "+port)