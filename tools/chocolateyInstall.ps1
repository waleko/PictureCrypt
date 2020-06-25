$name = 'PictureCrypt'
$installerType = 'exe'
$url  = 'https://storage.googleapis.com/waleko-personal.appspot.com/PictureCrypt-setup.exe'
$silentArgs = '/S'

Install-ChocolateyPackage $name $installerType $silentArgs $url