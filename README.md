# Steganography Method: Mask application on Least Significant bits (LSB)
When hiding information in the least significant bits of an image, uniform areas such as the sky tend to reveal the introduced noise more easily. To mitigate this effect and improve concealment, this algorithm primarily modifies pixels in non-uniform areas of the image.

## Detecting Non-Uniform Areas
To differentiate pixels in uniform regions from those in high-contrast areas, a median() filter is applied. This function computes the median intensity of the target pixel and its 8 neighboring pixels. Then, a threshold is set:

If the difference between the pixel’s value and the median is large enough, the pixel is considered part of a non-uniform region and can be altered without noticeable visual distortion.

## Encoding the Data
To ensure proper decoding of the hidden information, a marking mechanism is implemented:

The least significant bit (LSB) of each pixel is used as an indicator:

**•** It is set to 1 if the median filter detects noise in the surrounding area.
**•** It is set to 0 otherwise.

The hidden data is then **inserted into the subsequent bits determined by the order parameter**, but only in the pixel bytes where the least significant bit is 1

Here is the resulting image obtained with an **order of 5** with different thresholds, along with the altered pixels containing data highlighted in red right below.

![](https://github.com/AxelPatron/Steganography-Optimization/blob/main/docs/filtered.jpg)

With a threshold of 5, we can already observe that the sky remains visually unaltered, making the image modifications nearly imperceptible. With a threshold of **10**, the changes become completely unnoticeable as you can see with the red pixels.

## Decoding the Hidden Text

The process of decoding the hidden text involves extracting the information that was previously embedded into the least significant bits (LSB) of the image. To begin decoding, the first step is to identify which pixels contain the hidden data. This can be done by examining the least significant bit of each pixel. The pixels where the LSB is set to **1** (as indicated by the median filter in the encoding phase) are the ones that hold the concealed information.

To retrieve the hidden data, the following steps are followed:

**Extracting the LSB:** 
For each byte of the image, the least significant bit is extracted and stored.
**•** Reconstructing the Data: The extracted bits are then used to identify wich byte contains the hidden data. The length of the data is often determined by the size of the image and the encoding scheme used.
**•** Converting to Text: Once the binary data is obtained, it is concatenated and converted back into readable text by interpreting the binary sequence according to a specific character encoding scheme (such as ASCII or UTF-8).

Since the steganography method used in this process involves only altering the pixels in non-uniform areas (where noise is introduced), the surrounding pixels in uniform regions remain unchanged. **This ensures that the decoding process is robust, with minimal visual distortion, even after the extraction of the hidden text.**
