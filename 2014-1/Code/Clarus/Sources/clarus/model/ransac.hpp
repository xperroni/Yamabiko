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

#ifndef RANSAC_HPP
#define RANSAC_HPP

#include <clarus/core/list.hpp>

#include <opencv2/opencv.hpp>

template<class Model> class ransac {
    typedef clarus::List<Model> Models;

    typedef clarus::ListIterator<Model> ModelIterator;

    typedef typename Model::Datum Datum;

    typedef clarus::List<Datum> Data;

    typedef clarus::ListIterator<Datum> DataIterator;

    typedef clarus::ListIteratorConst<Datum> DataIteratorConst;

    static cv::RNG rng;

    static void add(Models &models, const Model &model, const double &far);

    static int fitness(const Model &model, const double &near, const Data &data);

    static void next(ModelIterator &n, Models &models, const Model &model, const double &far);

    static void remove(Data &samples, const Model &model, const double &near);

    static void sample(Data& samples, size_t n, const Data &data);

public:
    static void fit(Model &model, const Data &data, int seed, const double &near, int rounds);

    static void fit(
        Models &models, size_t upto, const double &separation,
        const Data &data, size_t seed, const double &near, int rounds,
        const Model &base = Model()
    );

    static void fit(
        Models &models, const double &separation,
        const Data &data, size_t seed, const double &near, int rounds
    );
};

template<class Model> cv::RNG ransac<Model>::rng;

template<class Model> void ransac<Model>::add(Models &models, const Model &model, const double &far) {
    for (ModelIterator i(models); i.more();) {
        if (model.distance(i.next()) < far) {
            return;
        }
    }

    models.append(model);
}

template<class Model> void ransac<Model>::next(
    ModelIterator &n,
    Models &models,
    const Model &model,
    const double &far
) {
    for (ModelIterator i(models); i != n;) {
        if (model.distance(i.next()) < far) {
            return;
        }
    }

    n.next();
}

template<class Model> void ransac<Model>::remove(Data &samples, const Model &model, const double &near) {
    Data outliers;
    for (DataIterator j(samples); j.more();) {
        Datum &datum = j.next();
        if (model.distance(datum) > near) {
            outliers.append(datum);
        }
    }

    samples = outliers;
}

template<class Model> int ransac<Model>::fitness(
    const Model &model,
    const double &near,
    const Data &data
) {
    Data consensus;
    for (DataIteratorConst i(data); i.more();) {
        const Datum &datum = i.next();
        if (model.distance(datum) <= near) {
            consensus.append(datum);
        }
    }

    return consensus.size();
}

template<class Model> void ransac<Model>::sample(Data& samples, size_t n, const Data &data) {
    int m = data.size();
    for (size_t i = 0; i < n; i++) {
        int index = rng(m);
        samples.append(data[index]);
    }
}

template<class Model> void ransac<Model>::fit(
    Model &model,
    const Data &data,
    int seed,
    const double &near,
    int rounds
) {
    Model tentative;
    int best = fitness(model, near, data);

    for (int i = 0; i < rounds; i++) {
        Data samples;
        sample(samples, seed, data);
        tentative.fit(samples);

        int count = fitness(tentative, near, data);
        if (count > best) {
            best = count;
            model = tentative;
        }
    }

    model.fitness = best;
}

template<class Model> void ransac<Model>::fit(
    Models &models, size_t upto, const double &separation,
    const Data &data, size_t seed, const double &near, int rounds,
    const Model &base
) {
    Data samples = data;
    while (samples.size() >= seed && models.size() < upto) {
        Model model(base);
        fit(model, samples, seed, near, rounds);
        remove(samples, model, near);
        add(models, model, separation);
    }
}

template<class Model> void ransac<Model>::fit(
    Models &models, const double &separation,
    const Data &data, size_t seed, const double &near, int rounds
) {
    Data samples = data;
    for (ModelIterator i(models); i.more() && samples.size() >= seed;) {
        Model &model = i.next();
        fit(model, samples, seed, near, rounds);
        remove(samples, model, near);
        next(i, models, model, separation);
    }
}

#endif
