require 'fileutils'

directory 'vendor/gen' do
  Dir.chdir 'vendor' do
    sh 'rake'
  end
end

directory 'gen' do |t|
  FileUtils.mkdir_p t.name
  
  Dir.chdir(t.name) do
    sh 'cmake ..'
  end
end

task :default => :build

desc 'Build'
task :build => ['vendor/gen','gen'] do
  Dir.chdir('gen') do
    sh 'make example'
  end
end

desc 'Test'
task :test => ['vendor/gen','gen'] do
  Dir.chdir('gen') do
    options = '--output-on-failure' 
    options += " -R #{ENV['t']}" unless ENV['t'].nil?
    sh "make spec_bin test ARGS='#{options}'"
  end
end

desc 'Spec'
task :spec => ['vendor/gen','gen'] do
  Dir.chdir('gen') do
    sh 'make spec'
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
