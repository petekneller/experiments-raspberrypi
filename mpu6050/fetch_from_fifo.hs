module Main where

import System.IO.SMBus
import Data.Bits
import Data.Time.Clock
import Text.Printf

dev = Device 0x68

fetchFifoCount bus dev = do
  hi <- read_byte_data bus dev '\x72'
  lo <- read_byte_data bus dev '\x73'
  return $ (fromEnum hi) `shiftL` 8 + (fromEnum lo)

fetchFifo bus dev = read_byte_data bus dev '\x74'

doIt bus dev = do
  cnt <- fetchFifoCount bus dev
  t1 <- getCurrentTime
  _ <- mapM_ (\_ -> fetchFifo bus dev) $ take cnt (repeat 1)
  t2 <- getCurrentTime
  let delta = (realToFrac ((utctDayTime t2) - (utctDayTime t1))) :: Double
  putStrLn $ printf "Fetched %4d bytes from FIFO and took %f seconds" cnt delta

main = withSMBus 1 $ \bus -> do
  _ <- write_byte_data bus dev '\x6B' '\x00' -- wake up device
  _ <- write_byte_data bus dev '\x23' '\x40' -- enable gyro X to FIFO
  _ <- write_byte_data bus dev '\x6A' '\x40' -- enable FIFO
  _ <- write_byte_data bus dev '\x1A' '\x00' -- set DLPF off
  _ <- write_byte_data bus dev '\x19' ((toEnum 7) :: Char) -- set sampling divider = x + 1
  mapM_ (\_ -> doIt bus dev) $ repeat 1
