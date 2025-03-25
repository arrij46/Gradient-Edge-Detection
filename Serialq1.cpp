#define FileName "data1/flower.jpg"
#define STB_IMAGE_IMPLEMENTATION
#define VERBOSE 1
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include "stb_image.h"
#include <vector>
using namespace std;

void PrintImg(vector<vector<float>> img, int rows, int cols);
bool ReadFile(int &rows, int &cols, int &channels, vector<vector<float>> &img);
void SaveImage(vector<vector<float>> &img, int rows, int cols, const char *filename);
void Prewitt(vector<vector<float>> img, int rows, int cols);
int main()
{
    int rows, cols, channels;
    vector<vector<float>> img;
    // Read the image
    if (!ReadFile(rows, cols, channels, img))
    {
        cout << "Error reading file" << endl;
        return -1;
    }
    if (VERBOSE)
    {
        cout << "ROWS: " << rows << ", COLS: " << cols << endl;
        cout << "Image matrix:" << endl;
        // PrintImg(img, rows, cols);
    }
    Prewitt(img,rows,cols);
    return 0;
}
void PrintImg(vector<vector<float>> img, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cout << img[i][j] << " ";
        }
        cout << endl;
    }
}
bool ReadFile(int &rows, int &cols, int &channels, vector<vector<float>> &img)
{
    unsigned char *image = stbi_load(FileName, &cols, &rows, &channels, 1);

    if (image == NULL)
    {
        cout << "Failed to load image";
        return 0;
    }

    // Matrix
    // Resize img to match image dimensions
    img.resize(rows, vector<float>(cols));
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            img[i][j] = float(image[i * cols + j]);
        }
    }
    stbi_image_free(image);
    return 1;
}
void SaveImage(vector<vector<float>> &img, int rows, int cols, const char *filename)
{
    unsigned char *output_image = new unsigned char[rows * cols];

    // Convert matrix to 1D array
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            output_image[i * cols + j] = static_cast<unsigned char>(img[i][j] * 255); // Scale to 0-255
        }
    }

    // Save as PNG
    stbi_write_png(filename, cols, rows, 1, output_image, cols);

    delete[] output_image; // Free memory
}
void Prewitt(vector<vector<float>> img, int rows, int cols)
{
    vector<vector<float>> Magnitude(rows, vector<float>(cols,0.0));
    vector<vector<float>> Gx = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
    vector<vector<float>> Gy = {{1, 1, 1}, {0, 0, 0}, {-1, -1, -1}};
    float Threshold = 0.0;

    // Convolution
    float sumx = 0.0, sumy = 0.0;
    for (int i = 1; i < rows - 1; i++)
    {
        for (int j = 1; j < cols - 1; j++)
        {
            sumx = 0.0, sumy = 0.0;
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    sumx += img[i + k][j + l] * Gx[k + 1][l + 1];
                    sumy += img[i + k][j + l] * Gy[k + 1][l + 1];
                }
            }
            // Magnitude
            Magnitude[i][j] = sqrt(sumx * sumx + sumy * sumy);
            Threshold += Magnitude[i][j];
        }
    }
    // Threshold
    Threshold = Threshold / (rows * cols - 4);

    // Edge Matrix
    for (int i = 1; i < rows - 1; i++)
    {
        for (int j = 1; j < cols - 1; j++)
        {
            if (Magnitude[i][j] > Threshold)
                Magnitude[i][j] = 0.0;
            else
                Magnitude[i][j] = 1.0;
        }
    }
    //PrintImg(Magnitude, rows, cols);
    
    SaveImage(Magnitude, rows, cols, "output.png");
}
