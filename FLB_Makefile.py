# 2018 Hiroyuki Ogasawara
# vim:ts=4 sw=4 et:

#  $ flmake
#  $ flmake run
#  $ more output_log.txt
#  $ flamke list
#
# debug build
#  $ flamke debug
#
# Show CPU information
#  $ ./vfpbench -i

import re

#------------------------------------------------------------------------------

src_list= [
        'src/linux_main.cpp',
        'src/TestBase.cpp',
        'src/SSE32SP_Test.cpp',
        'src/SSE32DP_Test.cpp',
        'src/SSE64SP_Test.cpp',
        'src/SSE64DP_Test.cpp',
        'src/VFP32SP_Test.cpp',
        'src/VFP32DP_Test.cpp',
        'src/VFP64SP_Test.cpp',
        'src/VFP64DP_Test.cpp',
        'src/VFP64HP_Test.cpp',
        'src/MIPSSP_Test.cpp',
        'src/MIPSDP_Test.cpp',
        'src/ResultData.cpp',
        'src/BenchApplication.cpp',
        'src/BenchmarkTest.cpp',
        'src/minilib/SystemAPI.cpp',
        'src/minilib/Thread.cpp',
        'src/minilib/MemoryAllocator.cpp',
        'src/minilib/Matrix4.cpp',
        'src/minilib/Math.cpp',
        'src/minilib/SystemInfo.cpp',
        'src/minilib/ConsoleLog.cpp',
        'src/minilib/PlatformString.cpp',
        'src/minilib/DateTime.cpp',
    ]

#        'src/MatrixTest_SSESP.cpp',
#        'src/MatrixTest_VFP32SP.cpp',
#        'src/MatrixTest_VFP64SP.cpp',

TargetName= 'vfpbench'

#------------------------------------------------------------------------------

tool.addCleanTask( genv, 'clean' )

#------------------------------------------------------------------------------

def get_arm64_arch( env ):
    global re
    if env.getTargetPlatform() == 'Linux':
        pat_fphp= re.compile( r'\bfphp\b' )
        pat_asimdhp= re.compile( r'\basimdhp\b' )
        with open( '/proc/cpuinfo', 'r' ) as fi:
            for line in fi:
                pat1= pat_fphp.search( line )
                pat2= pat_asimdhp.search( line )
                if pat1 and pat2:
                    return  'armv8.2-a+simd+fp16'
    return  'armv8-a+simd'

#------------------------------------------------------------------------------

env= tool.createTargetEnvironment()
env.addIncludePaths( ['src'] )

if env.getHostPlatform() == 'macOS':
    env.addCCFlags( ['-DFL_PRESET_OSX=1'] )
elif env.getHostPlatform() == 'Linux':
    env.addCCFlags( ['-DFL_PRESET_LINUX=1'] )

env.refresh()

def addCustomBuild( env, TargetName, src_list, config ):
    is_termux= False
    if env.getHostPlatform() == 'Linux':
        is_termux= env.isTermux()
    tool= env.tool
    arch_list= env.getSupportArchList()
    task_list= []
    for arch in arch_list:
        local_env= env.clone()
        local_env.setConfig( config )
        local_env.setTargetArch( arch )
        if is_termux:
            if arch == 'arm7':
                local_env.addCCFlags( ['-mfpu=neon', '-mfloat-abi=softfp'] )
            elif arch == 'x64':
                local_env.setTargetArch( 'x86' )
        if arch == 'arm64':
                global get_arm64_arch
                local_env.addCCFlags( ['-march=' + get_arm64_arch( env ) ] )
        if config == 'Release':
            exe_name= TargetName
        else:
            local_env.addCCFlags( ['-DDEBUG=1'] )
            exe_name= env.getExeName( TargetName + '_' + env.getTargetArch() + '_' + env.getConfig() )
        local_env.refresh()
        task= tool.addExeTask( local_env, None, src_list, target=exe_name )
        task_list.append( task )
    return  task_list

task_list= addCustomBuild( env, TargetName, src_list, 'Release' )
tool.addGroupTask( env, 'build', task_list )

task_list= addCustomBuild( env, TargetName, src_list, 'Debug' )
tool.addGroupTask( env, 'debug', task_list )


#------------------------------------------------------------------------------

def BenchRun( task ):
    import  subprocess
    if os.path.exists( '.save.log' ):
        if not os.path.exists( 'save' ):
            os.mkdir( 'save' )
        global  re
        date_pat= re.compile( r'^T\s+"([0-9]+)\s+([0-9]+)"$' )
        nname= 'save0.log'
        with open( '.save.log', 'r' ) as fi:
            for line in fi:
                pat= date_pat.search( line )
                if pat is not None:
                    fdate= pat.group( 1 )
                    ftime= pat.group( 2 )
                    nname= 'save_%s_%s.log' % (fdate, ftime)
                    break
        os.rename( '.save.log', os.path.join( 'save', nname ) )
    if os.path.exists( 'vfpbench' ):
        proc= subprocess.Popen( ['./vfpbench'] )
        proc.wait()

task= tool.addScriptTask( env, 'run', BenchRun )


#------------------------------------------------------------------------------

def PushLog( task ):
    global re
    import datetime
    src_file= 'output_log.txt'
    #cdate= datetime.datetime.today().strftime( '%Y%m%d_%H%M%S' )
    fdate= datetime.datetime.fromtimestamp( os.stat( src_file ).st_mtime ).strftime( '%Y%m%d_%H%M%S' )
    with open( src_file, 'r' ) as fi:
        name= None
        name_pat= re.compile( '^Name: (.*)$' )
        for line in fi:
            pat= name_pat.search( line )
            if pat is not None:
                name= pat.group(1).strip()
    if os.path.exists( '.processor_name' ):
        with open( '.processor_name', 'r' ) as fp:
            name= fp.read().strip()
    if not os.path.exists( 'log' ):
        os.mkdir( 'log' )
    #file_name= os.path.join( 'log', '%s_%s.txt' % (name, fdate) )
    file_name= os.path.join( 'log', '%s.txt' % name )
    import shutil
    shutil.copy( src_file, file_name )
    print( file_name )

task= tool.addScriptTask( env, 'push', PushLog )


#------------------------------------------------------------------------------

def ListLog( task ):
    log_list= os.listdir( 'log' )
    global re
    import urllib.parse
    plist= [
            re.compile( r'^SingleThread\s+HP\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^SingleThread\s+SP\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^SingleThread\s+DP\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^MultiThread\s+HP\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^MultiThread\s+SP\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^MultiThread\s+DP\s+max:\s+([0-9.-]+)' ),
        ]
    thread_pat= re.compile( r'^CPU\s+Thread:\s+([0-9]+)' )
    core_pat=   re.compile( r'^CPU\s+Core\s+:\s+([0-9]+)' )
    clock_pat=  re.compile( r'^\s+Group\s+[0-9]+\s*:.*Clock=\s*([0-9.-]+)\s*GHz' )
    device_list= []
    for log in log_list:
        score= []
        min_clock= 1e30
        max_clock= 0
        with open( os.path.join( 'log', log ), 'r' ) as fi:
            for line in fi:
                for p in plist:
                    pat= p.search( line )
                    if pat is not None:
                        flops= pat.group(1)
                        if flops == '-':
                            score.append( 0.0 )
                        else:
                            score.append( float(flops) )
                        break
                pat= thread_pat.search( line )
                if pat is not None:
                    cpu_thread= int(pat.group(1))
                pat= core_pat.search( line )
                if pat is not None:
                    cpu_core= int(pat.group(1))
                pat= clock_pat.search( line )
                if pat is not None:
                    cpu_clock= float(pat.group(1))
                    if cpu_clock < min_clock:
                        min_clock= cpu_clock
                    if cpu_clock > max_clock:
                        max_clock= cpu_clock
        name,_= os.path.splitext( log )
        device_list.append( (name,score,log,cpu_thread,cpu_core,min_clock,max_clock) )
    device_list_sp= sorted( device_list, key=lambda a: a[1][4], reverse=True )
    if task.table:
        print( '^ Device  ^ Clock ^ Thread ^  Single Thread                      ^^^  Multi Thread                     ^^^' )
        print( '^ :::     ^ :::   ^ :::    ^  Half-p    ^   Single-p  ^  Double-p  ^  Half-p   ^  Single-p  ^  Double-p  ^' )
    else:
        print( 'Device                                              Clock  C/T      S-HP     S-SP     S-DP      M-HP     M-SP     M-DP' )
    for name,sc,log,core,thread,mic,mac in device_list_sp:
        if task.table:
            url= 'https://github.com/hiroog/vfpbench/blob/master/log/' + urllib.parse.quote( log )
            line= '| [[%s|%-70s]]  |  %5.3f GHz |  %d/%d |  %8.3f |  %8.3f |  %8.3f |  %8.3f |  %8.3f |  %8.3f |' % (url,name[:70], mac, thread, core, sc[0], sc[1], sc[2], sc[3], sc[4], sc[5])
            print( line.replace( '0.000', '--' ) )
        else:
            print( '%-50s  %5.3f %2d/%2d %8.3f %8.3f %8.3f  %8.3f %8.3f %8.3f' % (name[:50], mac, thread, core, sc[0], sc[1], sc[2], sc[3], sc[4], sc[5]) )

task= tool.addScriptTask( env, 'list', ListLog )
task.table= False
task= tool.addScriptTask( env, 'table', ListLog )
task.table= True


#------------------------------------------------------------------------------
class Converter:
    def __init__( self ):
        global re
        self.date_pat= re.compile( r'(.+)_([0-9]+)_([0-9]+)\.txt' )

    def convet_filename_to_header( self, root, file_name ):
        pat= self.date_pat.search( file_name );
        if pat is None:
            print( 'ERROR', file_name )
            return
        fname= pat.group( 1 )
        fdate= pat.group( 2 )
        ftime= pat.group( 3 )
        full_path= os.path.join( root, file_name )
        new_name= os.path.join( 'tlog', fname + '.txt' )
        with open( full_path, 'r' ) as fi:
            with open( new_name, 'w' ) as fo:
                fo.write( 'Date: %s %s\n' % (fdate,ftime) )
                for line in fi:
                    fo.write( line )

    def walk( self ):
        for root,dirs,files in os.walk( 'log' ):
            for file_name in files:
                self.convet_filename_to_header( root, file_name )




