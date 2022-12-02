const express = require('express');
const router = express.Router();
const pool = require('../src/database')

router.get('/', (req, res) => {
    /* Codigo para obtenr pagina principal */
    res.json({
        hola: "si"
    });
})

router.get('/nosotros', (req, res) => {
    /* Codigo para obtenr pagina principal */
})

router.post('/newSample', (req, res) => {
    res.send('Simon');
    const {id_maquina, perimetro, area, porcentajeAzul, porcentajeVerde, porcentajeRojo, calidad} = req.body 
    nuevoSample = {
        'id_Muestra': 'NULL', 
        'id_Maquina': id_maquina, 
        'perimetro': perimetro, 
        'area': area,
        'porcentajeAzul': porcentajeAzul, 
        'porcentajeVerde': porcentajeVerde, 
        'porcentajeRojo': porcentajeRojo, 
        'calidad': calidad,
        'fecha': '25-11-2022'
    }
    console.log("Aqui");
    // pool.query(`INSERT INTO 'muestra' ('id_Muestra', 'id_Maquina', 'peso', 'diametro', 'calidad','fecha') VALUES (NULL, '${id_maquina}', '${peso}', '${diametro}', '${calidad}','12-11-2022');`, {})
    pool.query(`INSERT INTO muestra set ?`, nuevoSample);
})

module.exports = router;