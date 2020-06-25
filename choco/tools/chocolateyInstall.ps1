$name = 'PictureCrypt'
$installerType = 'exe'
$url  = '.\PictureCrypt-setup.exe'
$silentArgs = '/S'

Install-ChocolateyPackage $name $installerType $silentArgs $url