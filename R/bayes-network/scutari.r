### OBIETTIVO: PREVISIONE DELLA "RESTITUIBILITA" DI UN PRESTITO
library(bnlearn)

## load the data.
credit =
  read.csv("german_credit.csv", header = TRUE,
    colClasses = c("factor", "factor", "numeric", "factor", "factor", "numeric",
      "factor", "factor", "factor", "factor", "factor", "factor", "factor",
      "numeric", "factor", "factor", "factor", "factor", "factor", "factor",
      "factor"))

## rename variables for easy reference and better plotting.
# Creditability                      -> CRED
# Account Balance                    -> BAL
# Duration of Credit (Months)        -> DCM
# Payment Status of Previous Credit  -> PSPC
# Purpose                            -> PURP
# Credit Amount                      -> CRAM
# Value Savings & Stocks             -> VSST
# Length of Current Employment       -> EMPL
# Instalment %                       -> INST
# Sex and Marital Status             -> SEX
# Guarantors                         -> GUAR
# Duration in Current Address        -> ADDR
# Most Valuable Available Asset      -> MVAA
# Age (Years)                        -> AGE
# Concurrent Credits                 -> CCR
# Type of Apartment                  -> APA
# No of Credits at this Bank         -> BACR
# Occupation                         -> OCC
# No of Dependents                   -> NDEP
# Telephone                          -> TEL
# Foreign Worker                     -> FORE
names(credit) = c("CRED", "BAL", "DCM", "PSPC", "PURP", "CRAM", "VSST", "EMPL",
                  "INST", "SEX", "GUAR", "ADDR", "MVAA", "AGE", "CCR", "APA",
                  "BACR", "OCC", "NDEP", "TEL", "FORE")

## recode response for easy reference.
levels(credit$CRED) = c("BAD", "GOOD")

## discretise continuous variables in three quantiles.
### More quantiles -> more parameters to the corresponding variable
dcredit = discretize(credit, method = "quantile")

## build a plain logistic regression model, on the ORIGINAL data.
### prima cosa che dovrebbero saper fare tutti. L'idea e'
# MODELLARE CREDITABILITY RISPETTO ALLE ALTRE VARIABILI
### si rileva che una buona meta' delle variabili non sono assolutamente
# rilevanti per spiegare lo stato del prestito
lreg = glm(CRED ~ ., data = credit, family = binomial)
# check whether the explanatory variables are significant.
anova(lreg, test = "Chisq")

## build a second logistic regression model, on the DISCRETIZED data.
### si rileva che la situazione NON cambia particolarmente. Ma con altri dati (ed altri quanti) poteva cambiare parecchio...
### e' buona cosa controllare sempre. se cambiavano di tanto, era ipotizzabile la COLLINEARITÀ della variabile originale
lreg2 = glm(CRED ~ ., data = dcredit, family = binomial)
# the subset of significant variables is almost the same as before, so the discretisation has a limited impact on the results.
anova(lreg2, test = "Chisq")

## 1 --- build a naive Bayes classifier.
## ha struttura predeterminata: tutte le frecce puntano DALLA variabile risposta (CRED). prima pagina del PDF
# define the structure of the network.
nB = naive.bayes(dcredit, training = "CRED")
# plot the network.
graphviz.plot(nB, shape = "ellipse", layout = "fdp")
# fit the parameters on the network using all the data. 'bayes' metodo che prevede una stima un po' diversa dalle brutali frequenze relative
nBfitted = bn.fit(nB, dcredit, method = "bayes")
# look at the model structure.
nBfitted
# check which arcs, if removed, make BIC decrease; the subset of variables thus identified is again very similar to the above.
# così facendo, la qualità del modello cala parecchio.
arc.strength(nB, data = dcredit, criterion = "bic")

### tabella  CRED/BAL contiene PROB CONDIZIONATE (somma per ogni colonna = 1)


## 2 --- build a tree-augmented naive Bayes (TAN) classifier;							seconda pagina del PDF
## unlike naive Bayes, it does not assume the predictors are independent of each other. they are linked by arcs to form a tree
## le variabili esplicative NON son più INdipendenti
## inoltre non ci son più tabelle singole (le variabili spesso hanno più di un arco entrante)
TAN = tree.bayes(dcredit, training = "CRED")
# plot the network.
graphviz.plot(TAN, shape = "ellipse", layout = "dot")
# fit the parameters on the network using all the data.
TANfitted = bn.fit(TAN, dcredit, method = "bayes")
# look at the model structure.
TANfitted
# Domanda: sto generando miliardi di tabelle. ne valeva la pena? guardo BIC; è aumentato rispetto a Naive Bayes, quindi è meglio TAN
# compare with naive Bayes, using AIC and BIC; why the difference?
AIC(nBfitted, dcredit)
AIC(TANfitted, dcredit)

BIC(nBfitted, dcredit)
BIC(TANfitted, dcredit)

## compare the two classifiers with cross-validation
## everything including the network structure has to be learned under cross-validation to avoid inflated goodness-of-fit estimates.
bn.cv(dcredit, bn = "naive.bayes", algorithm.args = list(training = "CRED"), fit = "bayes")
bn.cv(dcredit, bn = "tree.bayes", algorithm.args = list(training = "CRED"), fit = "bayes")
# naive Bayes appears to be slightly better, use multiple runs of CROSS-VALIDATION to make sure.
nBerr = numeric(50)
TANerr = numeric(50)

for (run in 1:50) {

  xval = bn.cv(dcredit, bn = "naive.bayes", fit = "bayes",
           algorithm.args = list(training = "CRED"))
  nBerr[run] = attr(xval, "mean")

}#FOR

for (run in 1:50) {

  xval = bn.cv(dcredit, bn = "tree.bayes", fit = "bayes",
           algorithm.args = list(training = "CRED"))
  TANerr[run] = attr(xval, "mean")

}#FOR

mean(nBerr)
mean(TANerr)
# plot the distributions of the classification errors for the two models. terza pagina del PDF
# STATISTICAMENTE, il modello rosso è molto meglio del blu, ma all'atto PRATICO non conta molto questo.
d1 = density(nBerr)
d2 = density(TANerr)
plot(range(d1$x, d2$x), range(d1$y, d2$y), type = "n", xlab = "x", ylab = "Density")
lines(d1, col = "red")
lines(d2, col = "blue")

## prognostic model (explanatory variables on top).					quarta pagina del PDF
## qui NON DICO NIENTE; decido solo che CRED sta in fondo (non esistono archi che escono da cred)
# with hill-climbing, non causal.
bl = tiers2blacklist(list(names(dcredit)[-1], "CRED"))
bn.hc = hc(dcredit, blacklist = bl)
graphviz.plot(bn.hc, highlight = list(nodes = "CRED"))
# the only nodes connected to CRED are BAL and VSST; does the model have a reasonable classification error with only 2 variables ?
# ciò ci sta dicendo di buttare via una dozzina di variabili !!!
bn.cv(dcredit, bn = "hc", loss = "pred", algorithm.args = list(blacklist = bl), loss.args = list(target = "CRED"), fit = "bayes")
# both are also among the significant regression coefficients in the logistic regression
lreg.p = p.adjust(anova(lreg2, test = "Chisq")$`Pr(>Chi)`, method = "fdr")[-1]
names(dcredit)[-1][lreg.p < 0.01]
# ci abbiamo rimesso circa un 6% sull'errore di previsione; non tremendo ma peggiore comunque.

# Bayesian predictions vs frequentist predictions.
# shuffle the data to get unbiased splits.
kcv = split(sample(nrow(credit)), 1:10)

# a turno, 9 sono training e 1 è test
predicted = lapply(kcv, function(test) {

  # split training and test.
  dtraining = dcredit[-test, ]
  dtest = dcredit[test, ]
  # fit the model on the training set.
  m = bn.fit(hc(dtraining, blacklist = bl), dtraining, method = "bayes")
  # Bayesian (posterior) predictions for the test set.
  post = predict(m, node = "CRED", data = dtest, method = "bayes-lw", n = 2000)

  return(list(obs = dtest$CRED, post = post))

})
# l'idea di Naive Bayes di predirre a posteriori anzichè in modo classico è vincente (?)
predicted = data.frame(
  OBS = do.call(c, lapply(predicted, `[[`, "obs")),
  PRED = do.call(c, lapply(predicted, `[[`, "post"))
)

length(which(predicted$OBS != predicted$PRED))/nrow(predicted)

## pezzo saltato
# with HITON + hill-climbing, partly causal.
bn.rsmax2 = rsmax2(dcredit, blacklist = bl, restrict = "si.hiton.pc", maximize = "hc")
graphviz.plot(bn.rsmax2, highlight = list(nodes = "CRED"))
# predictive performance is about the same as above.
bn.cv(dcredit, bn = "rsmax2", loss = "pred", fit = "bayes",
  algorithm.args = list(blacklist = bl, restrict = "si.hiton.pc", maximize = "hc"),
  loss.args = list(target = "CRED"))

## diagnostic model. 		sesta pagina del PDF
## l'informazione a posteriori riconverge verso CRED da quasi ovunque :-)
bl = tiers2blacklist(list("CRED", names(dcredit)[-1]))
# with hill-climbing, non causal.
bn.hc2 = hc(dcredit, blacklist = bl)
graphviz.plot(bn.hc2, highlight = list(nodes = "CRED"))
# cross-validated classification error is still bad...
bn.cv(dcredit, bn = "hc", loss = "pred", fit = "bayes", algorithm.args = list(blacklist = bl), loss.args = list(target = "CRED"))
# and the there is no correspondence between e.g. BIC and error rates.
BIC(bn.hc, data = dcredit)
BIC(bn.hc2, data = dcredit)
BIC(bn.rsmax2, data = dcredit)

# Bayesian predictions vs frequentist predictions (the former are better this time).
# shuffle the data to get unbiased splits.
kcv = split(sample(nrow(credit)), 1:10)

predicted = lapply(kcv, function(test) {

  # split training and test.
  dtraining = dcredit[-test, ]
  dtest = dcredit[test, ]
  # fit the model on the training set.
  m = bn.fit(hc(dtraining, blacklist = bl), dtraining, method = "bayes")
  # Bayesian (posterior) predictions for the test set.
  post = predict(m, node = "CRED", data = dtest, method = "bayes-lw", n = 2000)

  return(list(obs = dtest$CRED, post = post))
})
# l'errore è ritornato a 0.266: abbastanza buono
predicted = data.frame(
  OBS = do.call(c, lapply(predicted, `[[`, "obs")),
  PRED = do.call(c, lapply(predicted, `[[`, "post"))
)

length(which(predicted$OBS != predicted$PRED))/nrow(predicted)

## X --- use only the nodes in the Markov blanket of CRED; predictive power is about
## the same because the Markov blanket isolates CRED from the rest of the network,
## so it is equivalent to condition on all nodes or just on the Markov blanket in the predictions.
bl = tiers2blacklist(list("CRED", mb(bn.hc2, "CRED")))
kcv = split(sample(nrow(credit)), 1:10)
## MB: CRED, VSST, BAL, PSPC, TEL, DCM
## graphviz.plot(bn.hc2, highlight = list(nodes = c("CRED", mb(bn.hc2, "CRED"))))
# come numeri non dovremmo trovare parecchia differenza, ma abbiamo eliminato un sacco di dipendenze irrilevanti !
predicted = lapply(kcv, function(test) {

  # split training and test.
  dtraining = dcredit[-test, c("CRED", mb(bn.hc2, "CRED"))]
  dtest = dcredit[test, c("CRED", mb(bn.hc2, "CRED"))]
  # fit the model on the training set.
  m = bn.fit(hc(dtraining, blacklist = bl), dtraining, method = "bayes")
  # Bayesian (posterior) predictions for the test set.
  post = predict(m, node = "CRED", data = dtest, method = "bayes-lw", n = 2000)

  return(list(obs = dtest$CRED, post = post))
})

predicted = data.frame(
  OBS = do.call(c, lapply(predicted, `[[`, "obs")),
  PRED = do.call(c, lapply(predicted, `[[`, "post"))
)

length(which(predicted$OBS != predicted$PRED))/nrow(predicted)

## purely causal model, starting with no blacklist.
# Inter-IAMB (set debug = TRUE to see what the algorithm is doing!).
bn.iamb = inter.iamb(dcredit)
graphviz.plot(bn.iamb, highlight = list(nodes = "CRED"))
# the network is not completely directed, extend it to a DAG (by picking a DAG
# in the equivalence class we learned from the data).
bn.iamb = cextend(bn.iamb)
graphviz.plot(bn.iamb, highlight = list(nodes = "CRED"))
# the Markov blanket of CRED should make it independent of the other predictors;
# and that is why it comprises the same nodes.
names(dcredit)[-1][lreg.p < 0.01]
mb(bn.iamb, "CRED")
# note that the Markov blanket of CRED is about the same in the prognostic model
# but not in the diagnostic model.
mb(bn.hc, "CRED")
mb(bn.hc2, "CRED")

# some of the arcs are noise, let's do model averaging to try and remove them
# from the model.
arcs = boot.strength(dcredit, algorithm = "inter.iamb")
avg = averaged.network(arcs)
graphviz.plot(avg, highlight = list(nodes = "CRED"))
# the Markov blanket of CRED has changed from before.
mb(avg, "CRED")
