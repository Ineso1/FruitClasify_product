import cv2 as cv
import numpy as np

def binarize(sample):
    _, _, redLayer = cv.split(sample)
    redBilateral = cv.bilateralFilter(redLayer, 15, 35, 25)
    _, redBilateral_thresh = cv.threshold(redBilateral, 150, 255, cv.THRESH_OTSU)
    retval, labels, stats, centroids = cv.connectedComponentsWithStats(redBilateral_thresh)
    area_min = 200
    for i in range(0, retval):
        area = stats[i, cv.CC_STAT_AREA]
        if area < area_min:
            indices = np.where(labels == i)
            redBilateral_thresh[indices] = 0
    return redBilateral_thresh

def sampleDrawDetect(sample, binary, contours, rect):
    muestraContorno = cv.drawContours(sample.copy(), contours, -1, (0,255,0), 3)
    box = cv.boxPoints(rect)
    box = np.int0(box)
    muestraContorno = cv.drawContours(muestraContorno, [box], 0, (0,0,255), 2)
    cv.imshow('Min rectangulo', muestraContorno)


def colorStatistics(sample):
    colors = ('b', 'g', 'r')
    for i, col in enumerate(colors):
        hist = cv.calcHist([sample], [i], None, [256], [0,256])

def sampleLecture(fileName):
    sample = cv.imread(fileName)
    binary = binarize(sample)
    cv.imshow('Imagen binarizada', binary)
    contours, _ = cv.findContours(binary, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
    perimetro = cv.arcLength(contours[0], True)
    area = cv.contourArea(contours[0])
    print(f"El perimetro de la naranja es de: {perimetro} pixeles\nEl area de la naranja es de {area} pixeles")
    rect = cv.minAreaRect(contours[0])
    sampleDrawDetect(sample, binary, contours, rect)
    colorStatistics(sample)

def main():
    sampleLecture('src/Naranja Buena Calidad (4).jpeg')    

if __name__ == '__main__':
    main()
    cv.waitKey(0)