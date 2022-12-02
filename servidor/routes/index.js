const express = require('express');
const router = express.Router();
const fs = require('fs');
const filePath = "./dataSet/prueba.jpeg";
const { spawn } = require('child_process');


router.post('/newSample', (req, res) => {
    
    var ImageFile = fs.createWriteStream(filePath, {encoding: 'utf8'});
    req.on('data', (data) =>{
        ImageFile.write(data);
    })

    obj = { funciona : "si"}
    const childPython = spawn('python', ['./Clasify/prueba.py', JSON.stringify(obj)]);
    childPython.stdout.on('data', (data)=> {
        console.log(`Imagen procesada correctamente: ${data}`);
        res.send(data);
    });

    childPython.stderr.on('data', (data)=>{
        console.error(`stderr: ${data}`);
        res.send(data);
    });

    req.on('end', async function(){
        ImageFile.end();
        /*res.json({ respuesta: 'Obtenida' });*/
    });
});

/*
Return:
    Circunferemcia de la naranja
    Diametro de la naranja
    Area de la naranja
    Madurez de la naranja
*/

module.exports = router;