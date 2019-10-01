setwd("C:/Users/Patrizio/Desktop")
library(lattice)

# d <- data.frame(index=1:20, x=rnorm(20), y=rnorm(20))
# d <- read.table("mytable.csv")
# head(d, n=3)
# xyplot(x+y~index, data=d, type='l', auto.key=list(space='right'))

mydata <- read.csv("program_full.csv", header=FALSE)
matplot(mydata[, 1], mydata[, -1], type="l")
