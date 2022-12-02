const express = require('express')
const app = express();
const routes = require('./routes/index');
const bodyParser = require('body-parser');
const {spawn} = require('child_process');

//Puerto de servidor
app.set('port', process.env.PORT || 5000)

//Middlewares
app.use((req, res, next) => {
    console.log(`${req.url} -${req.method}`);
    next();
});

//Rutas Server
app.use(routes)


//Escucha del server
app.listen(app.get('port'), () => {
    console.log('Server on port ', app.get('port'))
});