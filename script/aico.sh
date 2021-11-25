#!/bin/sh
#File Name    : aico.sh
#Author       : aico
#Mail         : 2237616014@qq.com
#Github       : https://github.com/TBBtianbaoboy
#Site         : https://www.lengyangyu520.cn
#Create Time  : 2021-11-25 10:46:53
#Description  : 

#:for debugging and controlling script behavior
set -eu
#:check script but not execute,below is example.
#bash -n main.sh
runMusic(){
  cd $HOME/.local/share/ncm_aico
  ./netease_cloud_music_aico
}

main(){
  runMusic
  exit
}
main $@

