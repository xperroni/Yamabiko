/*
Copyright (c) Helio Perroni Filho <xperroni@gmail.com>

This file is part of Clarus.

Clarus is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Clarus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Clarus. If not, see <http://www.gnu.org/licenses/>.
*/

#include <clarus/model/munkres.hpp>

#include <clarus/core/list.hpp>
using clarus::List;
using clarus::ListIterator;

#include <clarus/model/point.hpp>
using clarus::Point;
using clarus::Point2D;

#include <stdexcept>

static const int NORMAL = 0;
static const int STAR = 1;
static const int PRIME = 2;

class Munkres {
    cv::Mat_<bool> row_mask;

    cv::Mat_<bool> col_mask;

    cv::Mat mask_matrix;

    cv::Mat matrix;

    int saverow, savecol;

    inline bool find_uncovered_in_matrix(double item, int &row, int &col) const;

    int step1();

    int step2();

    int step3();

    int step4();

    int step5();

    int step6();

public:
    Munkres(cv::Mat &matrix, cv::Mat &mask_matrix);
};

inline double minimum(const cv::Mat &data) {
    double minVal, maxVal;
    cv::minMaxLoc(data, &minVal, &maxVal);
    return minVal;
}

inline void minimize_along_direction(cv::Mat &matrix, bool over_columns) {
    // Look for a minimum value to subtract from all values along
    // the "outer" direction.
    for (int i = 0,  n = matrix.rows; i < n; i++) {
        cv::Mat streak = (over_columns ? matrix.col(i) : matrix.row(i));
        double min = minimum(streak);
        if (min > 0) {
            streak -= min;
        }
    }
}


bool Munkres::find_uncovered_in_matrix(double item, int &row, int &col) const {
    int size = matrix.rows;

    for (row = 0; row < size ; row++ ) {
        if (row_mask(row)) {
            continue;
        }

        for (col = 0; col < size; col++) {
            if (col_mask(col)) {
                continue;
            }

            if (matrix.at<double>(row, col) == item) {
                return true;
            }
        }
    }

    return false;
}

int Munkres::step1() {
    int size = matrix.rows;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (matrix.at<double>(row, col) != 0) {
                continue;
            }

            bool starred = false;
            for (int row2 = 0; row2 < size; row2++) {
                if (mask_matrix.at<int>(row2, col) == STAR) {
                    starred = true;
                    break;
                }
            }

            if (starred) {
                continue;
            }

            for (int col2 = 0; col2 < size; col2++) {
                if (mask_matrix.at<int>(row, col2) == STAR) {
                    starred = true;
                    break;
                }
            }

            if (!starred) {
                mask_matrix.at<int>(row, col) = STAR;
            }
        }
    }

  return 2;
}

int Munkres::step2() {
    int size = matrix.rows;
    int covercount = 0;

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (mask_matrix.at<int>(row, col) == STAR) {
                col_mask(col) = true;
                covercount++;
            }
        }

        if (covercount >= size) {
            return 0;
        }
    }

    return 3;
}

int Munkres::step3() {
    /*
    Main Zero Search

    1. Find an uncovered Z in the distance matrix and prime it. If no such zero exists, go to Step 5
    2. If No Z* exists in the row of the Z', go to Step 4.
    3. If a Z* exists, cover this row and uncover the column of the Z*. Return to Step 3.1 to find a new Z
    */

    if (find_uncovered_in_matrix(0, saverow, savecol)) {
        mask_matrix.at<int>(saverow,savecol) = PRIME; // prime it.
    }
    else {
        return 5;
    }

    for (int col = 0, size = matrix.rows; col < size; col++) {
        if (mask_matrix.at<int>(saverow, col) == STAR) {
            row_mask(saverow) = true; //cover this row and
            col_mask(col) = false; // uncover the column containing the starred zero
            return 3; // repeat
        }
    }

    return 4; // no starred zero in the row containing this primed zero
}

int Munkres::step4() {
    // seq contains pairs of row/column values where we have found
    // either a star or a prime that is part of the ``alternating sequence``.
    List<Point> seq;

    // use saverow, savecol from step 3.
    Point z0 = Point2D(saverow, savecol);
    seq.append(z0);

    // We have to find these two pairs:
    Point z1 = Point2D(-1, -1);
    Point z2n = Point2D(-1, -1);

    /*
    Increment Set of Starred Zeros

    1. Construct the ``alternating sequence'' of primed and starred zeros:

    Z0 : Unpaired Z' from Step 4.2
    Z1 : The Z* in the column of Z0
    Z[2N] : The Z' in the row of Z[2N-1], if such a zero exists
    Z[2N+1] : The Z* in the column of Z[2N]

    The sequence eventually terminates with an unpaired Z' = Z[2N] for some N.
    */

    int row = 0;
    int col = savecol;
    int size = matrix.rows;
    bool madepair = false;
    do {
        madepair = false;
        for (row = 0 ; row < size; row++ ) {
            if (mask_matrix.at<int>(row, col) == STAR) {
                z1[0] = row;
                z1[1] = col;
                if (seq.contains(z1)) {
                    continue;
                }

                madepair = true;
                seq.append(z1);
                break;
            }
        }

        if (!madepair) {
            break;
        }

        madepair = false;

        for (col = 0; col < size; col++) {
            if (mask_matrix.at<int>(row, col) == PRIME) {
                z2n[0] = row;
                z2n[1] = col;
                if (seq.contains(z2n)) {
                    continue;
                }

                madepair = true;
                seq.append(z2n);
                break;
            }
        }
    } while (madepair);

    for (ListIterator<Point> i(seq); i.more();) {
        const Point &p = i.next();

        // 2. Unstar each starred zero of the sequence.
        if (mask_matrix.at<int>(p[0], p[1]) == STAR) {
            mask_matrix.at<int>(p[0], p[1]) = NORMAL;
        }

        // 3. Star each primed zero of the sequence,
        // thus increasing the number of starred zeros by one.
        if (mask_matrix.at<int>(p[0], p[1]) == PRIME) {
            mask_matrix.at<int>(p[0], p[1]) = STAR;
        }
    }

    // 4. Erase all primes, uncover all columns and rows,
    for (row = 0; row < size; row++) {
        for (col = 0 ; col < size; col++) {
            if (mask_matrix.at<int>(row, col) == PRIME) {
                mask_matrix.at<int>(row, col) = NORMAL;
            }
        }
    }

    row_mask = false;
    col_mask = false;

    // and return to Step 2.
    return 2;
}

int Munkres::step5() {
    /*
    New Zero Manufactures

    1. Let h be the smallest uncovered entry in the (modified) distance matrix.
    2. Add h to all covered rows.
    3. Subtract h from all uncovered columns
    4. Return to Step 3, without altering stars, primes, or covers.
    */

    double h = 0;
    int size = matrix.rows;
    for (int row = 0; row < size; row++) {
        if (row_mask(row)) {
            continue;
        }

        for (int col = 0; col < size; col++) {
            if (col_mask(col)) {
                continue;
            }

            if (
                (h > matrix.at<double>(row, col) && matrix.at<double>(row, col) != 0) ||
                h == 0
            ) {
                h = matrix.at<double>(row, col);
            }
        }
    }

    for (int row = 0; row < size; row++) {
        if (!row_mask(row)) {
            continue;
        }

        for (int col = 0 ; col < size; col++) {
            matrix.at<double>(row, col) += h;
        }
    }

    for (int col = 0; col < size; col++) {
        if (col_mask(col)) {
            continue;
        }

        for (int row = 0; row < size; row++) {
            matrix.at<double>(row, col) -= h;
        }
    }

    return 3;
}

/*
*
* Linear assignment problem solution
* [modifies matrix in-place.]
*
* Assignments are STAR values in the mask matrix.
*/
Munkres::Munkres(cv::Mat &_matrix, cv::Mat &_mask_matrix):
    row_mask(1, _matrix.rows, false),
    col_mask(1, _matrix.rows, false),
    mask_matrix(_mask_matrix),
    matrix(_matrix)
{
    // Prepare the matrix values...
    minimize_along_direction(matrix, false);
    minimize_along_direction(matrix, true);

    // Follow the steps
    for (int step = 1; step != 0;) {
        switch (step) {
            // Next step is always 2
            case 1: step = step1(); break;

            // Next step is either 0 or 3
            case 2: step = step2(); break;

            // Next step in [3, 4, 5]
            case 3: step = step3(); break;

            // Next step is always 2
            case 4: step = step4(); break;

            // Next step is always 3
            case 5: step = step5(); break;

            default: break;
        }
    }
}

List<Point> clarus::munkres(const cv::Mat &data, bool rowfirst) {
    size_t rows = data.rows;
    size_t cols = data.cols;

    if (rows < cols) {
        throw std::runtime_error("Munkres method requires cols <= rows");
    }

    cv::Mat costs;
    if (rows == cols) {
        costs = data.clone();
    }
    else {
        double minVal, maxVal;
        cv::minMaxLoc(data, &minVal, &maxVal);
        costs = cv::Mat(rows, rows, CV_64F, cv::Scalar(maxVal));
        cv::Rect roi(0, 0, cols, rows);
        cv::Mat values(costs, roi);
        data.copyTo(values);
    }

    cv::Mat solution(costs.size(), CV_32S, cv::Scalar(NORMAL));
    Munkres solver(costs, solution);

    List<Point> assignments;
    for (size_t i = 0; i < rows; i++) {
        int *cell = solution.ptr<int>(i);
        for (size_t j = 0; j < cols; j++, cell++) {
            if (*cell != STAR) {
                continue;
            }

            if (rowfirst) {
                assignments.append(Point2D(i, j));
            }
            else {
                assignments.append(Point2D(j, i));
            }
        }
    }

    return assignments;
}
