import sys
import cv2 as cv
import numpy as np

def binarize(sample):
    blueLayer, greenLayer, redLayer = cv.split(sample)
    redBilateral = cv.bilateralFilter(redLayer, 15, 35, 25)
    _, redBilateral_thresh = cv.threshold(redBilateral, 150, 255, cv.THRESH_OTSU)
    retval, labels, stats, centroids = cv.connectedComponentsWithStats(redBilateral_thresh)
    area_min = 200
    for i in range(0, retval):
        area = stats[i, cv.CC_STAT_AREA]
        if area < area_min:
            indices = np.where(labels == i)
            redBilateral_thresh[indices] = 0
    return [redBilateral_thresh, blueLayer, greenLayer, redLayer]

def sampleDrawDetect(sample, binary, contours, rect):
    muestraContorno = cv.drawContours(sample.copy(), contours, -1, (0,255,0), 3)
    box = cv.boxPoints(rect)
    box = np.int0(box)
    muestraContorno = cv.drawContours(muestraContorno, [box], 0, (0,0,255), 2)
    #cv.imshow('Min rectangulo', muestraContorno)


def colorStatistics(blueLayer, greenLayer, redLayer, binaryMask):
    blueMask = cv.bitwise_or(blueLayer, blueLayer, mask = binaryMask)
    greenMask = cv.bitwise_or(greenLayer, greenLayer, mask = binaryMask)
    redMask = cv.bitwise_or(redLayer, redLayer, mask = binaryMask)
    suma = 0
    cant = 0
    resT = ""
    for i in blueMask:
        for j in i:
            if j != 0:
                cant += 1
                suma += j
    res = suma/(2.55*cant)
    resT += str(res) + ","

    suma = 0
    cant = 0
    for i in greenMask:
        for j in i:
            if j != 0:
                cant += 1
                suma += j
    res = suma/(2.55*cant)
    resT += str(res) + ","



    suma = 0
    cant = 0
    for i in redMask:
        for j in i:
            if j != 0:
                cant += 1
                suma += j
    res = suma/(2.55*cant)
    resT += str(res)

    return resT


def sampleLecture(fileName):
    sample = cv.imread(fileName)
    binary, blueLayer, greenLayer, redLayer = binarize(sample)
    #cv.imshow('Imagen binarizada', binary)
    contours, _ = cv.findContours(binary, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
    perimetro = cv.arcLength(contours[0], True)
    area = cv.contourArea(contours[0])
    rect = cv.minAreaRect(contours[0])
    #sampleDrawDetect(sample, binary, contours, rect)
    colorS = colorStatistics(blueLayer, greenLayer, redLayer, binary)
    print(f"{perimetro},{area},{colorS}")


def main():
    sampleLecture('dataSet/Naranja Buena Calidad (0).jpeg')
    #sampleLecture('dataSet/prueba.jpeg')


if __name__ == '__main__':
    main()
    #cv.waitKey(0)