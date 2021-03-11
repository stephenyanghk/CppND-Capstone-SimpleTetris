#ifndef TETRIMINO_H
#define TETRIMINO_H

class Tetrimino
{
    public:
        Tetrimino();

        int TetriminoId() const { return mTetriminoId; }

        int RotationId() const { return mRotationId; }
        void RotationId(int rotationId) { mRotationId = rotationId; }

        int X() const { return mX; }
        int Y() const { return mY; }

        void X(int x) { mX = x; }
        void Y(int y) { mY = y; }

        int colorR() const { return mTetriminoRGBA[mTetriminoId][0]; }
        int colorG() const { return mTetriminoRGBA[mTetriminoId][1]; }
        int colorB() const { return mTetriminoRGBA[mTetriminoId][2]; }
        int colorA() const { return mTetriminoRGBA[mTetriminoId][3]; }

        void setValues(int tetriminoId, int rotationId, int x, int y);

        static int GetType(int pX, int pY, int tetriminoId, int rotationId);
        static int GetInitOffsetX(int tetriminoId, int rotationId);
        static int GetInitOffsetY(int tetriminoId, int rotationId);

    private:
        int mTetriminoId;
        int mRotationId;
        int mX;
        int mY;

        const static int mTetrimino[7][4][5][5];
        const static int mTetriminoInitOffset[7][4][2];
        const static int mTetriminoRGBA[7][4];

};

#endif // TETRIMINO_H