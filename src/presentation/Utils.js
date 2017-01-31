function binaryIndexOf(searchElement, compareFunct, sizeFunct, getFunct) {
    var minIndex = 0
    var maxIndex = sizeFunct() - 1
    var currentIndex
    var currentElement

    while (minIndex <= maxIndex) {
        currentIndex = (minIndex + maxIndex) / 2 | 0
        currentElement = getFunct(currentIndex)

        if (compareFunct(currentElement, searchElement) < 0) {
            minIndex = currentIndex + 1
        } else if (compareFunct(currentElement, searchElement) > 0) {
            maxIndex = currentIndex - 1
        } else {
            return currentIndex
        }
    }

    return -1
}

function formatSize(value) {
    var unit = "bytes"
    if (value > 1024) {
        value = value / 1024
        unit = "KiB"
    }

    if (value > 1024) {
        value = value / 1024
        unit = "MiB"
    }

    if (value > 1024) {
        value = value / 1024
        unit = "GiB"
    }

    return "" + Math.round(value, 2) + " " + unit
}
