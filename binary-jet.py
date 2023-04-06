import numpy as np
from numpy.core.arrayprint import printoptions
import pandas as pd
import xgboost as xgb
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from hipe4ml.model_handler import ModelHandler
from hipe4ml.tree_handler import TreeHandler
from hipe4ml.analysis_utils import train_test_generator
from hipe4ml import plot_utils


quarkH = TreeHandler('jetinfo.root','tree_quark')
datasetH = TreeHandler('dataset.root', 'tree_dataset')
gluonH = TreeHandler('jetinfo.root', 'tree_gluon')
#signalとbackgroundを合体させてtotal setを作るらしい。
train_test_data = train_test_generator([quarkH, gluonH], [1,0], test_size=0.5, random_state=42) #1がシグナルで0がバックグラウンドtest_sizeはテストセットとして使用される全セットの割合

#def train_test_generator
#(data_list, labels_list, sliced_df=False, **kwds)
#test_sizeでテスト用の割合
#機械学習のモデルの性能を比較するような場合、どのように分割されるかによって結果が異なってしまうため、乱数シードを固定して常に同じように分割されるようにする必要がある。
vars_to_draw = quarkH.get_var_names()


leg_labels = ['gluon', 'quark']

plot_utils.plot_distr([gluonH, quarkH], vars_to_draw, bins=100, labels=leg_labels, log=False, density=False, figsize=(12, 7), alpha=0.3, grid=False)
plt.subplots_adjust(left=0.06, bottom=0.06, right=0.99, top=0.96, hspace=0.55, wspace=0.55)
plot_utils.plot_corr([gluonH, quarkH], vars_to_draw, leg_labels)

#plt.show()

features_for_train = vars_to_draw.copy()
features_for_train.remove('partonID') #目的の特徴はtrainにいれない
features_for_train.remove('pTD') #目的の特徴はtrainにいれない


model_clf = xgb.XGBClassifier()
model_hdl = ModelHandler(model_clf, features_for_train)

hyper_pars_ranges = {'n_estimators': (200, 1000), 'max_depth': (2, 4), 'learning_rate': (0.01, 0.1)}
model_hdl.optimize_params_optuna(train_test_data, hyper_pars_ranges, cross_val_scoring='roc_auc', timeout=120,n_jobs=-1, n_trials=100, direction='maximize')


model_hdl.train_test_model(train_test_data)

y_pred_train = model_hdl.predict(train_test_data[0], False)
y_pred_test = model_hdl.predict(train_test_data[2], False)

plt.rcParams["figure.figsize"] = (10, 7)

ml_out_fig = plot_utils.plot_output_train_test(model_hdl, train_test_data, 100, False, leg_labels, True, density=True)
roc_train_test_fig = plot_utils.plot_roc_train_test(train_test_data[3], y_pred_test, train_test_data[1], y_pred_train, None, leg_labels)

datasetH.apply_model_handler(model_hdl, False)

#ここでセレクションの閾値を決めれる
selected_data_hndl = datasetH.get_subset('model_output>0.7')

labels_list = ["after selection", "before selection", "real quark"]
colors_list = ['red', 'blue', 'green']

fig = plt.figure()

ax = plt.gca()
ax1 = fig.add_subplot(2,2,1) #get current axes
plot_utils.plot_distr([selected_data_hndl, datasetH], column='nconst', bins=200, labels=labels_list, colors=colors_list, density=True, fill=True, histtype='step', alpha=0.3)
ax.set_xlabel('number of jet constituents')
ax.set_ylabel('Counts(Density)')
ax.margins(x=0)
ax.xaxis.set_label_coords(0.9, -0.075)

plot_utils.plot_distr([selected_data_hndl, datasetH], column='g', bins=500, labels=labels_list, colors=colors_list, density=True, fill=True, histtype='step', alpha=0.3)
ax2 = fig.add_subplot(2,2,2) #get current axes
ax = plt.gca()
ax.set_xlabel('g')
ax.set_ylabel('Counts(Density)')
ax.margins(x=0)
ax.xaxis.set_label_coords(0.9, -0.075)
ax.set_xlim([0,0.5])


plot_utils.plot_distr([selected_data_hndl, datasetH], column='pTD', bins=200, labels=labels_list, colors=colors_list, density=True, fill=True, histtype='step', alpha=0.3)
ax3 = fig.add_subplot(2,2,3) #get current axes
ax = plt.gca()
ax.set_xlabel('pTD')
ax.set_ylabel('Counts(Density)')
ax.margins(x=0)
ax.xaxis.set_label_coords(0.9, -0.075)

plot_utils.plot_distr([selected_data_hndl, datasetH], column='LeSub', bins=200, labels=labels_list, colors=colors_list, density=True, fill=True, histtype='step', alpha=0.3)
ax4 = fig.add_subplot(2,2,4) #get current axes
ax = plt.gca()
ax.set_xlabel('LeSub')
ax.set_ylabel('Counts(Density)')
ax.margins(x=0)
ax.xaxis.set_label_coords(0.9, -0.075)
ax.set_xlim([0,200])

plot_utils.plot_distr([selected_data_hndl, datasetH, quarkH], column='nconst', bins=200, labels=labels_list, colors=colors_list, density=True, fill=True, histtype='step', alpha=0.3)
ax = plt.gca()
ax.set_xlabel('nconst')
ax.margins(x=0)
ax.xaxis.set_label_coords(0.9, -0.075)


plot_utils.plot_distr([selected_data_hndl, datasetH], column='partonID', bins=2, labels=labels_list, colors=colors_list, density=False, fill=True, histtype='step', alpha=0.3)

selected_data_hndl.print_summary()
selected_data_df = selected_data_hndl.get_data_frame()
ID_list = selected_data_df['partonID'].to_list()
ID_len = len(ID_list)
n_g = 0

for i in ID_list:
    n_g += i
n_q = ID_len - n_g

# correct answer rate
car = n_q / ID_len
print ("n_q =" + str(n_q))
print("ML_quark =" + str(ID_len))
print("correct answer rate =" + str(car))



ax = plt.gca()
ax.set_title('CARresult', fontsize=0)
ax.set_xlabel('partonID',fontsize=20,fontweight='light')
ax.set_ylabel('counts', fontsize=20,fontweight='light')
# ax.margins(x=10)
ax.xaxis.set_label_coords(0.9, -0.075)
ax.set_xlim([-0.25,1.25])
ax.text(0.25, 1300, "quark", size=30, horizontalalignment="center", color="black",fontweight='light')
ax.text(0.75, 1600, "gluon", size=30, horizontalalignment="center", color="black",fontweight='light')
ax.tick_params(labelsize=30)
plt.show()
