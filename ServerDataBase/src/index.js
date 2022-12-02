const express = require('express');
const morgan = require('morgan');

const app = express();

app.set('port', process.env.PORT || 5000)

app.use(morgan('dev'));

app.use((req, res, next) => {
    next();
})

app.use(require('./routes/index'))

app.listen(app.get('port'), ()=>{ console.log('Sever on port ', app.get('port'))})
