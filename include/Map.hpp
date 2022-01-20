//
//  Map.hpp
//  Map Visualization
//
//  Created by William Lee on 11/19/21.
//

#ifndef Map_hpp
#define Map_hpp

#include <stdio.h>

class Map{
private:
    int wid;
    int hei;
    int *pix = nullptr;
public:
    Map();
    ~Map();

    void CleanUp(void);
    int GetHeight(void) const;
    int GetWidth(void) const;
    void Create(int w, int h);
    void SetPixel(int x,int y,int p);
    int GetPixel(int x,int y) const;
    void Draw(int pixelw,int pixelh) const;
};

#endif /* Map_hpp */
