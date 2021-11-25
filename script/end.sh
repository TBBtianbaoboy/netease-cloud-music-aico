#!/bin/sh
#File Name    : end.sh
#Author       : aico
#Mail         : 2237616014@qq.com
#Github       : https://github.com/TBBtianbaoboy
#Site         : https://www.lengyangyu520.cn
#Create Time  : 2021-11-24 21:22:07
#Description  : 

#:for debugging and controlling script behavior
set -eu
#:check script but not execute,below is example.
#bash -n main.sh
RED='\033[0;31m' #Red Color 
GREEN='\033[0;32m' #Green Color 
YELLOW='\033[1;33m' #Yellow Color 
NC='\033[0m' # No Color

gOutput(){
  echo -e "${GREEN}$1${NC}"
}

rOutput(){
  echo -e "${RED}$1${NC}"
}

yOutput(){
  echo -e "${YELLOW}$1${NC}"
}

print(){
  if [ ! -d $HOME/bin ];then 
    mkdir -p $HOME/bin
  fi
  cp netease_cloud_music_aico $HOME/.local/share/ncm_aico
  cp script/aico.sh $HOME/bin/aico
  gOutput "---------------------执行成功"
  yOutput "[5]##### 文件执行路径与配置文件路径如下: "
  echo "启动程序为: ~/bin/aico "
  echo "配置文件在: ~/.config/ncm_aico/config.json "
  gOutput "---------------------结束"
}

main(){
  print 
  exit
}
main $@
