#!/bin/sh
#File Name    : configure.sh
#Author       : aico
#Mail         : 2237616014@qq.com
#Github       : https://github.com/TBBtianbaoboy
#Site         : https://www.lengyangyu520.cn
#Create Time  : 2021-11-24 19:08:43
#Description  : 

#:for debugging and controlling script behavior
set -eu
#:check script but not execute,below is example.
#bash -n main.sh
DataDir=$HOME/.local/share/ncm_aico
ConfDir=$HOME/.config/ncm_aico
PcDir=/usr/lib64/pkgconfig

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

checkRoot(){
  if [[ $(id -u) != 0 ]];then
    rOutput "请以root权限运行配置脚本"
    return 2
  fi
}

checkPkgConfig(){
  if [[ $(command -v pkg-config) == "" ]];then
    rOutput "请安装必要的包管理工具:  pkg-config "
    return 4
  fi
}

checkNetwork(){
  yOutput "[1]###### 检查网络状态中..."
  local timeout=2
  server=baidu.com
  local ret_code=`curl -I -m 10 -o /dev/null -s -w %{http_code} ${server}`
  sleep 1
  if [ "$ret_code" = "200" ];then
    gOutput "-------------------------网络状态良好"
  else
    rOutput "-------------------------请确保网络连接正常再重试"
    return 3
  fi
}

createDir(){
  yOutput "[2]###### 配置项目目录中..."
  if [ ! -d $DataDir ];then
    mkdir -p $DataDir
  fi
  if [ ! -d $ConfDir ];then
    mkdir -p $ConfDir
  fi
  sleep 1
  gOutput "--------------------------目录配置完成"
}

mvDir(){
  yOutput "[3]###### 配置项目数据中..."
  # if [ ! -f /usr/lib64/libitemsaico.so ];then 
  #   sudo -u root sh -c 'cp ./lib/libitemsaico.so /usr/lib64/'
  # fi
  if [ ! -f $ConfDir/config.json ];then 
    cp ./config/config.json $ConfDir
  fi
  if [ ! -f $PcDir/ncm_aico.pc ];then 
    sudo -u root sh -c 'cp ./pkg_config/ncm-aico.pc /usr/lib64/pkgconfig/'
  fi
  sleep 1
  gOutput "--------------------------配置完成"
}

print(){
  yOutput "[4]###### 编译项目中..."
}

main(){
  # checkRoot
  checkPkgConfig
  checkNetwork
  createDir
  mvDir
  print
  exit 
}
main $@
