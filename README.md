# XNALangPatchTools
A tool chain for developing language patchs for XNA applications

```XNBFontRawDataProcessingTool```: Tool for processing raw data (```font.texture.png``` and ```font.yaml```). Can be used to edit / merge multiple data into 1 automatically.

```xnb_node.0.2.2```: A modified version of xnb node for processing large character set. (Due to the way data is inserted at the end of the buffer, the original tool has a time complexity of $O(n^2)$ regarding the character set size, making it impossible for the tool to complete the font library conversion for languages with large character sets such as Chinese.)

I have used this tool chain to develop a Chinese patch for Superfighters Deluxe.
