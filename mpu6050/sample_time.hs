module Main where

import Data.Time.Clock
import Data.Bits
import Text.Printf
import System.IO.SMBus
import System.Environment

dev = Device 0x68

sampleTime bus = do
  t1 <- getCurrentTime
  hi <- read_byte_data bus dev '\x43'
  lo <- read_byte_data bus dev '\x44'
  t2 <- getCurrentTime
  return $ ((realToFrac ((utctDayTime t2) - (utctDayTime t1))) :: Double)

main = withSMBus 1 $ \bus -> do
  args <- getArgs
  let n = (read $ head args) :: Int
  _ <- write_byte_data bus dev '\x6B' '\x00' -- wake up device
  times <- mapM (\_ -> sampleTime bus) $ take n (repeat 1)
  mapM (\time -> putStrLn $ printf "%f" time) times
