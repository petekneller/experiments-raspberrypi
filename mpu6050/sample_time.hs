module Main where

import System.IO.SMBus
import Data.Bits
import System.CPUTime
import Text.Printf
import Criterion.Main

dev = Device 0x68
sample = withSMBus 1 $ \bus -> do
  _ <- write_byte_data bus dev '\x6B' '\x00' -- wake up device
  hi <- read_byte_data bus dev '\x43'
  lo <- read_byte_data bus dev '\x44'
  return $ hi `shiftL` 8 + lo

main = defaultMain [
  bench "sample" $ nfIO sample
]
