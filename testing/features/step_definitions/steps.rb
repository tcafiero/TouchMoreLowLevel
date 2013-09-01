# encoding: utf-8
begin require 'rspec/expectations'; rescue LoadError; require 'spec/expectations'; end
require 'cucumber/formatter/unicode'


Given /^the two integer (.+) (.+)$/ do |a, b|
	@a = a.to_i
	@b = b.to_i
end

Given /^Operation (.+)$/ do |o|
	"End".should==o
end


When /^you make the sum$/ do
	@c=EmptyProject.Sum(@a,@b)
end

When /^you make the sub$/ do
	@c=EmptyProject.Sub(@a,@b)
end


Then /^the output should be (\d*)$/ do |expected|
		@c.should==expected.to_i
		@c=0
end

Then /^Flush Data$/ do
		EmptyProject.Flush
end

