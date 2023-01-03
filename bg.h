
//{{BLOCK(bg)

//======================================================================
//
//	bg, 240x160@8, 
//	Transparent palette entry: 2.
//	+ palette 256 entries, not compressed
//	+ 32 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 30x20 
//	Total size: 512 + 2048 + 1200 = 3760
//
//	Time-stamp: 2023-01-02, 18:36:23
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BG_H
#define GRIT_BG_H

#define mapWidth 30
#define mapHeight 20

#define bgTilesLen 2048
extern const unsigned short bgTiles[1024];

#define bgMapLen 1200
extern const unsigned short bgMap[600];

#define bgPalLen 512
extern const unsigned short bgPal[256];

#endif // GRIT_BG_H

//}}BLOCK(bg)
