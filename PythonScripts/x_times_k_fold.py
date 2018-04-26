import argparse
import pandas as pd
import numpy as np
from scipy.io import arff
from sklearn.model_selection import KFold
from os import listdir
from os.path import isfile, join
from sklearn import preprocessing
import os

def get_type(type):
    if type == "numeric":
        return "real"

def create_arff_file (path, fileName, data, meta):
    arffFile = open(join(path, fileName), 'w+')

    labels = data['class']
    labels = np.array(labels)

    arffFile.write("@relation {0}\n".format(meta.name))

    for i in xrange(len(meta.names())):
        attr = meta.names()[i]
        if attr != "class":
            arffFile.write("@attribute {0} {1}\n".format(attr, get_type(meta.types()[i])))
        else:
            arffFile.write("@attribute {0} {{".format(attr))
            sorted_labels = sorted(map(int, np.unique(labels)))
            arffFile.write("{0}".format(",".join(map(str, sorted_labels))))
            arffFile.write("}\n")

    arffFile.write("@data\n")

    data = np.array(data)
    for row in xrange(len(data)):
        arffFile.write(",".join(map(str, data[row])) + "\n")

def create_true_file (path, fileName, data, labels):
    trueFile = open(join(path, fileName), 'w+')

    dim = data.shape[1]

    trueFile.write("DIM={0};{1} TRUE\n".format(dim, fileName[:-5].upper()))

    unique_labels = sorted(map(int, np.unique(labels)))
    for label in unique_labels:
        line = " ".join(map(str, [1] * dim))

        newIndexes = np.where(labels == str(label))[0]

        line += " {0} ".format(len(newIndexes))
        line += " ".join(map(str, newIndexes))
        line += "\n"

        trueFile.write(line)

def create_xtimes_kfolds(xTimes, kFolds, folder, outputFolder):
    if not os.path.isdir(outputFolder + "_Train"): os.mkdir(outputFolder + "_Train")
    if not os.path.isdir(outputFolder + "_Test"): os.mkdir(outputFolder + "_Test")

    files = [f for f in listdir(folder) if isfile(join(folder, f))]
    files = sorted(files)

    for file in files:
        if ".arff" in file:
            data, meta = arff.loadarff(open(join(folder, file), 'rb'))
            data = pd.DataFrame(data)
            labels = data['class']
            del data['class']

            data = np.array(data)
            scaler = preprocessing.MinMaxScaler()
            scaler.fit(data)
            data = scaler.transform(data)
            labels = np.array(labels)

            for x in xrange(xTimes):
                kf = KFold(n_splits=kFolds, shuffle=True)
                fold = 0
                for train, test in kf.split(data):
                    train_data = data[train]
                    test_data = data[test]

                    train_labels = labels[train]
                    test_labels = labels[test]

                    trainFile = pd.DataFrame(train_data)
                    trainFile['class'] = train_labels
                    create_arff_file(path=outputFolder + "_Train", fileName="train_{0}_x{1}_k{2}.arff".format(file[:-5], x + 1, fold + 1),
                                     data=trainFile, meta=meta)
                    create_true_file(path=outputFolder + "_Train",
                                     fileName="train_{0}_x{1}_k{2}.true".format(file[:-5], x + 1, fold + 1),
                                     data=train_data, labels=train_labels)

                    testFile = pd.DataFrame(test_data)
                    testFile['class'] = test_labels
                    create_arff_file(path=outputFolder + "_Test", fileName="test_{0}_x{1}_k{2}.arff".format(file[:-5], x + 1, fold + 1),
                                     data=testFile, meta=meta)
                    create_true_file(path=outputFolder + "_Test",
                                     fileName="test_{0}_x{1}_k{2}.true".format(file[:-5], x + 1, fold + 1),
                                     data=test_data, labels=test_labels)

                    fold += 1

def create_true_files_from_path(folder):
    files = [f for f in listdir(folder) if isfile(join(folder, f))]
    files = sorted(files)

    for file in files:
        if ".arff" in file:
            data, meta = arff.loadarff(open(join(folder, file), 'rb'))
            data = pd.DataFrame(data)
            labels = data.iloc[:,-1]
            del data[data.columns[len(data.columns) - 1]]

            data = np.array(data)
            labels = np.array(labels)

            create_true_file(folder, "{0}.true".format(file[:-5]), data, labels)

parser = argparse.ArgumentParser()
parser.add_argument('-i', help='Input Directory', required=True)
parser.add_argument('-o', help='Output Directory', required=True)
parser.add_argument('-x', help='X Times', required=True, type=int)
parser.add_argument('-k', help='K Folds', required=True, type=int)
args = parser.parse_args()

folder = args.i
outputFolder = args.o
X = args.x
K = args.k

# create_true_files_from_path(folder)
create_xtimes_kfolds(xTimes=X, kFolds=K, folder=folder, outputFolder=outputFolder)