require 'fileutils'

directory 'gen'

task :default => :build

desc 'Build'
task :build => [:vendor, 'CMakeLists.txt'] do
  Dir.chdir('gen') do
    sh 'make'
    sh './main'
  end
end

task :vendor do
  Dir.chdir 'vendor' do
    sh 'rake'
  end
end

file 'CMakeLists.txt' => 'gen' do
  Dir.chdir('gen') do
    sh 'cmake ..'
  end
end

desc 'Clean'
task :clean => 'gen' do
  Dir.chdir('gen') do
    sh 'make clean'
  end
end

desc 'Rebuild'
task :rebuild => 'gen' do
  Dir.chdir('gen') do
    sh 'make clean all'
  end
end

desc 'Distclean'
task :distclean do
  FileUtils.rm_rf 'gen'
end
