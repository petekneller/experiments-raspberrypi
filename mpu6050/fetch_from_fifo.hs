module Main where

import System.IO.SMBus
import Data.Bits
import System.CPUTime
import Text.Printf

dev = Device 0x68

fetchFifoCount bus dev = do
  hi <- read_byte_data bus dev '\x72'
  lo <- read_byte_data bus dev '\x73'
  return $ (fromEnum hi) `shiftL` 8 + (fromEnum lo)

fetchFifo bus dev = read_byte_data bus dev '\x74'

doIt bus dev = do
  cnt <- fetchFifoCount bus dev
  _ <- mapM_ (\_ -> fetchFifo bus dev) $ take cnt (repeat 1)
  putStrLn $ printf "Fetched %d bytes from FIFO" cnt

main = withSMBus 1 $ \bus -> do
  _ <- write_byte_data bus dev '\x6B' '\x00' -- wake up device
  _ <- write_byte_data bus dev '\x23' '\x40' -- enable gyro X to FIFO
  _ <- write_byte_data bus dev '\x6A' '\x40' -- enable FIFO
  _ <- write_byte_data bus dev '\x1A' '\x00' -- set DLPF off
  _ <- write_byte_data bus dev '\x19' '\x07' -- set sampling divider = 8
  mapM_ (\_ -> doIt bus dev) $ repeat 1
