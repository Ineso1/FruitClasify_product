const mysql = require('mysql');
const {database} = require('./keys');
const {promisify} = require('util')

const pool = mysql.createPool(database);
pool.getConnection((err, connection)=>{
    if(err){
        if(err.code === 'PROTOCOL_CONNECTION_LOST'){
            console.error('desconeccion con la base de datos');
        }
    }
    if(connection) connection.release();
    console.log('DB conectada')
    return;
    
});
pool.query = promisify(pool.query);
module.exports = pool;