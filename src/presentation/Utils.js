function binaryIndexOf(searchElement, compareFunct, sizeFunct, getFunct) {
    var minIndex = 0;
    var maxIndex = sizeFunct() - 1;
    var currentIndex;
    var currentElement;

    while (minIndex <= maxIndex) {
        currentIndex = (minIndex + maxIndex) / 2 | 0;
        currentElement = getFunct(currentIndex);

        if (compareFunct(currentElement, searchElement) < 0) {
            minIndex = currentIndex + 1;
        }
        else if (compareFunct(currentElement, searchElement) > 0) {
            maxIndex = currentIndex - 1;
        }
        else {
            return currentIndex;
        }
    }

    return -1;
}
