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
        'src/minilib/ConsoleIO.cpp',
        'src/minilib/PlatformString.cpp',
        'src/minilib/SystemTime.cpp',
    ]

#        'src/MatrixTest_SSESP.cpp',
#        'src/MatrixTest_VFP32SP.cpp',
#        'src/MatrixTest_VFP64SP.cpp',

TargetName= 'vfpbench'

#------------------------------------------------------------------------------

def addCleanTask( env, task_name, dir_name= os.getcwd() ):
    def command( task ):
        import  BuildUtility
        BuildUtility.RemoveTree( os.path.join( task.cwd, task.env.OUTPUT_OBJ_DIR ) )
    clean_task= env.tool.addScriptTask( env, task_name, command )
    clean_task.cwd= os.getcwd()
    return  clean_task

addCleanTask( genv, 'clean' )


def makeExeName( env, src_file ):
    if env.getConfig() == 'Release':
        return  src_file
    if src_file:
        return  env.getExeName( src_file + '_' + env.getTargetArch() + '_' + env.getConfig() )
    return  '.'

#------------------------------------------------------------------------------

env= tool.createTargetEnvironment()
env.EXE_NAME_FUNC= makeExeName
env.addIncludePaths( ['src'] )

if env.getHostPlatform() == 'macOS':
    env.addCCFlags( ['-DflPRESET_OSX=1'] )
elif env.getHostPlatform() == 'Linux':
    env.addCCFlags( ['-DflPRESET_LINUX=1'] )


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
            #local_env.addCCFlags( ['-march=armv8a+fp16'] )
            local_env.addCCFlags( ['-march=armv8a'] )
            #local_env.addCCFlags( ['-march=armv8.2a+crypto+fp16+dotprod'] )
        if config != 'Release':
            local_env.addCCFlags( ['-DDEBUG=1'] )
        local_env.refresh()
        task= tool.addExeTask( local_env, TargetName, src_list )
        task_list.append( task )
    return  task_list

task_list= addCustomBuild( env, TargetName, src_list, 'Release' )
tool.addNamedTask( env, 'build', task_list )

task_list= addCustomBuild( env, TargetName, src_list, 'Debug' )
tool.addNamedTask( env, 'debug', task_list )


#------------------------------------------------------------------------------

def BenchRun( task ):
    import  subprocess
    if os.path.exists( 'vfpbench' ):
        proc= subprocess.Popen( ['./vfpbench'] )
        proc.wait()

task= tool.addScriptTask( env, 'run', BenchRun )


#------------------------------------------------------------------------------

def PushLog( task ):
    import re
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
    if name is None or name == '':
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
    import re
    plist= [
            re.compile( r'^SingleThread\s+HP\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^SingleThread\s+SP\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^SingleThread\s+DP\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^MultiThread\s+HP\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^MultiThread\s+SP\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^MultiThread\s+DP\s+max:\s+([0-9.-]+)' ),
        ]
    device_list= []
    for log in log_list:
        score= []
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
        name,_= os.path.splitext( log )
        device_list.append( (name,score) )
    device_list_sp= sorted( device_list, key=lambda a: a[1][4], reverse=True )
    if task.table:
        print( '^ Device  ^  Single Thread                     ^^^  Multi Thread                   ^^^' )
        print( '^ :::     ^  Half-p    ^   Single-p  ^  Double-p  ^  Half-p   ^  Single-p  ^  Double-p  ^' )
    for name,sc in device_list_sp:
        if task.table:
            line= '| %-70s  |  %8.3f |  %8.3f |  %8.3f |  %8.3f |  %8.3f |  %8.3f |' % (name[:70], sc[0], sc[1], sc[2], sc[3], sc[4], sc[5])
            print( line.replace( '0.000', '--' ) )
        else:
            print( '%-50s  %8.3f %8.3f %8.3f  %8.3f %8.3f %8.3f' % (name[:50], sc[0], sc[1], sc[2], sc[3], sc[4], sc[5]) )

task= tool.addScriptTask( env, 'list', ListLog )
task.table= False
task= tool.addScriptTask( env, 'table', ListLog )
task.table= True


#------------------------------------------------------------------------------
class Converter:
    def __init__( self ):
        import re
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




