const express = require('express');
const app = express();
const morgan = require('morgan');
const routes = require('../routes/index');

app.set('port', process.env.PORT || 6000);
app.use(morgan('dev'));


app.use((req, res, next) => {
    console.log(`${req.url} -${req.method}`);
    next();
});

app.use(express.json());

app.use(routes);

app.listen(app.get('port'), () => {
    console.log('Server on port ', app.get('port'));
})