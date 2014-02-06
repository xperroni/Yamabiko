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

#include <opencv2/opencv.hpp>

#include <vector>

template<class Model> class ransac {
    typedef std::vector<Model> Models;

    typedef typename std::vector<Model>::iterator ModelIterator;

    typedef typename Model::Datum Datum;

    typedef std::vector<Datum> Data;

    typedef typename Data::iterator DataIterator;

    typedef typename Data::const_iterator DataConstIterator;

    typedef typename Model::Distance Distance;

    static cv::RNG rng;

    static void add(Models &models, const Model &model, const Distance &far);

    static int fitness(const Model &model, const Distance &near, const Data &data);

    static void next(ModelIterator &n, Models &models, const Model &model, const Distance &far);

    static void remove(Data &samples, const Model &model, const Distance &near);

    static void sample(Data& samples, size_t n, const Data &data);

public:
    static void fit(Model &model, const Data &data, int seed, const Distance &near, int rounds);

    static void fit(
        Models &models, size_t upto, const Distance &separation,
        const Data &data, size_t seed, const Distance &near, int rounds,
        const Model &base = Model()
    );

    static void fit(
        Models &models, const Distance &separation,
        const Data &data, size_t seed, const Distance &near, int rounds
    );
};

template<class Model> cv::RNG ransac<Model>::rng;

template<class Model> void ransac<Model>::add(Models &models, const Model &model, const Distance &far) {
    for (ModelIterator i = models.begin(), n = models.end(); i != n; ++i) {
        if (model.distance(*i) < far) {
            return;
        }
    }

    models.push_back(model);
}

template<class Model> void ransac<Model>::next(
    ModelIterator &n,
    Models &models,
    const Model &model,
    const Distance &far
) {
    for (ModelIterator i = models.begin(); i != n; ++i) {
        if (model.distance(*i) < far) {
            return;
        }
    }

    ++n;
}

template<class Model> void ransac<Model>::remove(Data &samples, const Model &model, const Distance &near) {
    Data outliers;
    for (DataIterator j = samples.begin(), n = samples.end(); j != n; ++j) {
        Datum &datum = (*j);
        if (model.distance(datum) > near) {
            outliers.push_back(datum);
        }
    }

    samples = outliers;
}

template<class Model> int ransac<Model>::fitness(
    const Model &model,
    const Distance &near,
    const Data &data
) {
    Data consensus;
    for (DataConstIterator i = data.begin(), n = data.end(); i != n; ++i) {
        const Datum &datum = (*i);
        if (model.distance(datum) <= near) {
            consensus.push_back(datum);
        }
    }

    return consensus.size();
}

template<class Model> void ransac<Model>::sample(Data& samples, size_t n, const Data &data) {
    int m = data.size();
    for (size_t i = 0; i < n; i++) {
        int index = rng(m);
        samples.push_back(data[index]);
    }
}

template<class Model> void ransac<Model>::fit(
    Model &model,
    const Data &data,
    int seed,
    const Distance &near,
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
    Models &models, size_t upto, const Distance &separation,
    const Data &data, size_t seed, const Distance &near, int rounds,
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
    Models &models, const Distance &separation,
    const Data &data, size_t seed, const Distance &near, int rounds
) {
    Data samples = data;
    for (ModelIterator i = models.begin(), n = models.end(); i != n && samples.size() >= seed;) {
        Model &model = (*i);
        fit(model, samples, seed, near, rounds);
        remove(samples, model, near);
        next(i, models, model, separation);
    }
}

#endif
