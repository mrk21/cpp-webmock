require 'fileutils'

directory 'gen'

task :default => :build

desc 'Build'
task :build => 'gen' do
  Dir.chdir 'vendor' do
    sh 'rake'
  end
  
  Dir.chdir('gen') do
    sh 'cmake ..'
    sh 'make example'
  end
end

desc 'Test'
task :test => 'gen' do
  Dir.chdir 'vendor' do
    sh 'rake'
  end
  
  Dir.chdir('gen') do
    options = ENV['t'].nil? ? '--output-on-failure' : "-V -R #{ENV['t']}"
    sh 'cmake ..'
    sh "make build_test test ARGS='#{options}'"
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
