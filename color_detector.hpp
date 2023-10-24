class ColorDetector {
public:
    ColorDetector() {}

    virtual ~ColorDetector() {}

    // inout from the FE BZS
    void LoadImages();
    void FaceDetection();

    //resize the image to be able to create the neighborhood of the colors BT
    void ReadColors();
    //to check what format we need to create the Cube Object
    void CreateCube();
};
